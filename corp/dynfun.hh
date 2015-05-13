// $Id: dynfun.hh,v 2.0.1.1 2004-03-17 00:07:59 pary Exp $
//  Copyright (c) 1999-2003  Pavel Rychly

#ifndef DYNFUN_HH
#define DYNFUN_HH


class DynFun {
protected:
    void *fn;
public:
    DynFun () : fn (0) {}
    virtual ~DynFun (){}
    const char * operator () (const char *x);
    virtual const char * call (const char *x) =0;
};

DynFun *createDynFun (const char *type, const char *libpath, 
		      const char *funname, ...);

#endif
