// Copyright (c) 2001-2008  Pavel Rychly

#include <finlib/writelex.hh>
#include <finlib/consumer.hh>
#include "dynfun.hh"
#include "posattr.hh"
#include "corpconf.hh"
#include <iostream>


using namespace std;

// XXX dat nekam do finlib
inline void ensuresize (char *&ptr, int &allocsize, int needed, 
                        const char *where)
{
    if (allocsize >= needed) return;
    char *p = (char *)realloc (ptr, needed);
    if (!p)
        throw MemoryAllocationError (where);
    allocsize = needed;
    ptr = p;
}

void store_multival (int id, const char *str, const char *multisep, 
                     write_lexicon &wl, RevFileConsumer *rev)
{
    int did;
    if (!multisep || multisep == '\0') {
        // multisep = "" --> store characters
        char value[2];
        value[1] = 0;
        while (*str) {
            value[0] = *str;
            did = wl.str2id (value);
            rev->put (did, id);
        }
    } else {
        static int value_len = 0;
        static char *value = NULL;
        ensuresize (value, value_len, strlen (str) +1, "store_multival");
        value[0] = 0;
        strncat (value, str, value_len -1);
        for (char *s = strtok (value, multisep); s;
             s = strtok (NULL, multisep)) {
            did = wl.str2id (s);
            rev->put (did, id);
        }
    }
}


int main (int argc, char **argv) 
{
    if (argc < 3) {
        fputs ("usage: mkdynattr corpus dynattr\n", stderr);
        return 2;
    }
    try {
        CorpInfo *ci = loadCorpInfo (argv[1]);
        string attr = argv[2];
        int dotidx = attr.find ('.');
        if (dotidx >= 0) {
            // struct_name.attr_name
            string struct_name (attr, 0, dotidx);
            attr = string (attr, dotidx +1);
            ci = ci->find_struct (struct_name);
        }
        CorpInfo::MSS ao = ci->find_attr (attr);
        DynFun *fun = createDynFun (ao["FUNTYPE"].c_str(), 
                    ao["DYNLIB"].c_str(), ao["DYNAMIC"].c_str(),
                    ao["ARG1"].c_str(), ao["ARG2"].c_str());
        string path = ci->opts["PATH"] + attr;
        PosAttr *at = findPosAttr (ci, ao["FROMATTR"]);
        bool multival = ci->str2bool (ao["MULTIVALUE"]);
        char *multisep = NULL;
        if (multival)
            multisep = strdup (ao["MULTISEP"].c_str());
        delete ci;

        { // delete existing lexicons
        ToFile<int> lex (path + ".lex");
        ToFile<int> lexidx (path + ".lex.idx");
        ToFile<int> lexsrt (path + ".lex.srt");
        }
        long lexicon_size = 500000;
        if (!ao["LEXICONSIZE"].empty())
            lexicon_size = atol (ao["LEXICONSIZE"].c_str());
        write_lexicon wl (path, lexicon_size);
        RevFileConsumer *rev = RevFileConsumer::create (path);
        
        int id, id_range = at->id_range();
        int did;
        for (id = 0; id < id_range; id++) {
            if (at->freq(id) <= 0)
                continue;
            const char *str = (*fun)(at->id2str (id));
            did = wl.str2id (str);
            rev->put (did, id);
            if (multival)
                store_multival (id, str, multisep, wl, rev);
        }
        delete rev;
        delete at;
        delete fun;
    } catch (exception &e) {
        cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

