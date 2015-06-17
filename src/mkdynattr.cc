// Copyright (c) 2001-2015  Pavel Rychly, Milos Jakubicek

#include <finlib/writelex.hh>
#include <finlib/consumer.hh>
#include <finlib/revidx.hh>
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
        string path = ci->opts["PATH"];
        int dotidx = attr.find ('.');
        string struct_name = "";
        string attr_name = attr;
        if (dotidx >= 0) {
            // struct_name.attr_name
            struct_name = string (attr, 0, dotidx);
            path = ci->find_struct (struct_name)->opts["PATH"];
            struct_name += ".";
            attr_name = string (attr, dotidx +1);
        }
        CorpInfo::MSS ao = ci->find_attr (attr);
        DynFun *fun = createDynFun (ao["FUNTYPE"].c_str(), 
                    ao["DYNLIB"].c_str(), ao["DYNAMIC"].c_str(),
                    ao["ARG1"].c_str(), ao["ARG2"].c_str());
        path += attr_name;
        PosAttr *at = findPosAttr (ci, struct_name + ao["FROMATTR"]);
        bool multival = ci->str2bool (ao["MULTIVALUE"]);
        char *multisep = NULL;
        if (multival)
            multisep = strdup (ao["MULTISEP"].c_str());
        delete ci;

        long lexicon_size = 500000;
        if (!ao["LEXICONSIZE"].empty())
            lexicon_size = atol (ao["LEXICONSIZE"].c_str());
        write_lexicon wl (path, lexicon_size, false);
        RevFileConsumer *rev = RevFileConsumer::create (path);
        ToFile<lexpos> *ridx = NULL;
        if (!multival)
            ridx = new ToFile<lexpos>(path + ".lex.ridx");
        
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
            else
                ridx->put(did);
        }
        delete rev;
        delete fun;
        delete ridx;
        if (ao["DYNTYPE"] == "freq") {
            ToFile<int64_t> frqf (path + ".freq");
            map_delta_revidx revf (path);
            for (id = 0; id < revf.maxid(); id++) {
                NumOfPos count = 0;
                FastStream *fs = revf.id2poss (id);
                while (fs->peek() < fs->final())
                    count += at->freq (fs->next());
                delete fs;
                frqf.put (count);
            }
        } else if (id_range > 100 * wl.size())
            cerr << "warning: the ratio between the dynamic attribute lexicon"
                 << " and original lexicon is more than 1:100\nconsider"
                 << " changing DYNTYPE to 'freq'\n"
                 << "Source attribute lexicon size: " << id_range << "\n"
                 << "Dynamic attribute lexicon size: " << wl.size() << "\n";
        delete at;
    } catch (exception &e) {
        cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

