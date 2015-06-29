//  Copyright (c) 1999-2014  Pavel Rychly

#include "dynfun.hh"
#include "utf8.hh"
#include "excep.hh"
#include <cstdio>
#include <cstdarg>

#define LT_SCOPE        extern
#include <ltdl.h>

//-------------------- internal_funs --------------------

struct fun_item {
    const char *name;
    void *fn;
};

extern "C" {
#include "stddynfun.c"
}

const unsigned char * utf8lowercase (const unsigned char *src)
{
    return utf8_tolower (src);
}

const unsigned char * utf8uppercase (const unsigned char *src)
{
    return utf8_toupper (src);
}

#define DEFFN(x) {#x, (void *)x}
static fun_item internal_funs [] = {
    DEFFN(getnchar),
    DEFFN(getnextchar),
    DEFFN(getnextchars),
    DEFFN(getfirstn),
    DEFFN(getfirstbysep),
    DEFFN(getnbysep),
    DEFFN(getlastn),
    DEFFN(utf8getlastn),
    DEFFN(striplastn),
    DEFFN(lowercase),
    DEFFN(utf8lowercase),
    DEFFN(utf8uppercase),
    DEFFN(utf8capital),
    DEFFN(url2domain),
    DEFFN(ascii),
    {NULL, NULL}
};
#undef DEFFN

//-------------------- DynFun --------------------

const char * DynFun::operator () (const char *x) 
{
    static const char *defval = "";
    if (fn)
        return call (x);
    else
        return defval;
}

//-------------------- DynFun_base --------------------

class DynFun_base: public DynFun {
protected:
    lt_dlhandle module;
public:
    DynFun_base (const char *libpath, const char *funname);
    virtual ~DynFun_base();
};

static bool initialized_libltdl = false;

DynFun_base::DynFun_base (const char *libpath, const char *funname)
{
    if (strcmp (libpath, "internal") == 0) {
        module = 0;
        for (fun_item *i=internal_funs; i->name != NULL; i++)
            if (strcmp (funname, i->name) == 0) {
                fn = i->fn;
                return;
            }
        throw CorpInfoNotFound(std::string("Cannot find internal function ")
                               + funname);
    } else {
        throw NotImplemented("Dynamic external attributes are not supported.");
    }
}



DynFun_base::~DynFun_base() 
{
    if (module) 
        lt_dlclose (module);
}



//-------------------- DynFun_* --------------------

class DynFun_0: public DynFun_base {
public:
    typedef const char * (*type)(const char *);
    DynFun_0 (const char *libpath, const char *funname)
        : DynFun_base(libpath, funname) {}
    virtual const char * call (const char *x)
        {return type(fn)(x);}
};

template <class ArgType>
class DynFun_1: public DynFun_base {
public:
    typedef const char * (*type)(const char *, ArgType);
    ArgType op;
    DynFun_1 (const char *libpath, const char *funname, ArgType op)
        : DynFun_base(libpath, funname), op(op) {}
    virtual const char * call (const char *x)
        {return type(fn)(x, op);}
};

template <class Arg1Type, class Arg2Type>
class DynFun_2: public DynFun_base {
public:
    typedef const char * (*type)(const char *, Arg1Type, Arg2Type);
    Arg1Type op1;
    Arg2Type op2;
    DynFun_2 (const char *libpath, const char *funname, 
              Arg1Type op1, Arg2Type op2)
        : DynFun_base(libpath, funname), op1(op1), op2(op2) {}
    virtual const char * call (const char *x)
        {return type(fn)(x, op1, op2);}
};


typedef DynFun_1<int> DynFun_i;
typedef DynFun_1<char> DynFun_c;
typedef DynFun_1<const char*> DynFun_s;
typedef DynFun_2<int, int> DynFun_ii;
typedef DynFun_2<int, char> DynFun_ic;
typedef DynFun_2<int, const char*> DynFun_is;
typedef DynFun_2<char, int> DynFun_ci;
typedef DynFun_2<char, char> DynFun_cc;
typedef DynFun_2<char, const char*> DynFun_cs;
typedef DynFun_2<const char*, int> DynFun_si;
typedef DynFun_2<const char*, char> DynFun_sc;
typedef DynFun_2<const char*, const char*> DynFun_ss;


//-------------------- createDynFun --------------------

DynFun *createDynFun (const char *type, const char *libpath, 
                      const char *funname, ...)
{
    if (type[0] == '\0' || (type[1] == '\0' && type[0] == '0'))
        return new DynFun_0 (libpath, funname);

    const char *arg1, *arg2;
    va_list va;
    va_start(va, funname);
    arg1 = va_arg (va, const char *);
    if (type[1] == '\0') {
        va_end(va);
        switch (type[0]) {
        case 's':
            return new DynFun_s (libpath, funname, strdup (arg1));
        case 'i':
            return new DynFun_i (libpath, funname, atol (arg1));
        case 'c':
            return new DynFun_c (libpath, funname, arg1[0]);
        }
    } else {
        arg2 = va_arg (va, const char *);
        va_end(va);
        switch (type[0]) {
        case 's':
            switch (type[1]) {
            case 's':
                return new DynFun_ss (libpath, funname, 
                                      strdup (arg1), strdup (arg2)); 
            case 'i':
                return new DynFun_si (libpath, funname, 
                                      strdup (arg1), atol (arg2));
            case 'c':
                return new DynFun_sc (libpath, funname, 
                                      strdup (arg1), arg2[0]);
            }
            break;
        case 'i':
            switch (type[1]) {
            case 's':
                return new DynFun_is (libpath, funname, 
                                      atol (arg1), strdup (arg2)); 
            case 'i':
                return new DynFun_ii (libpath, funname, atol (arg1), atol (arg2));
            case 'c':
                return new DynFun_ic (libpath, funname, atol (arg1), arg2[0]);
            }
            break;
        case 'c':
            switch (type[1]) {
            case 's':
                return new DynFun_cs (libpath, funname, arg1[0], strdup (arg2));
            case 'i':
                return new DynFun_ci (libpath, funname, arg1[0], atol (arg2));
            case 'c':
                return new DynFun_cc (libpath, funname, arg1[0], arg2[0]);
            }
            break;
        }
    }
    return NULL;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
