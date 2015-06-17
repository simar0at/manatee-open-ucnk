// Copyright (c) 2004-2011  Pavel Rychly, Milos Jakubicek

#include <finlib/fromtof.hh>
#include "corpus.hh"
#include <iostream>
#include <cstdlib>

using namespace std;


int main (int argc, char **argv) 
{
    if (argc < 3) {
        cerr << "usage: mknorms CORPUS STRUCT [NORM_STRUCTATTR]\n"
             << "example: mknorms mycorp doc wordcount\n";
        return 2;
    }
    try {
        Corpus corp (argv[1]);
        Structure *st = corp.get_struct (argv[2]);
        NumOfPos *norms = new NumOfPos[st->size()];
        string normattrname;
        
        // init norms array
        cerr << "preparing norm counts ..." << endl;
        if (argc == 4) {
            normattrname = argv[3];
            PosAttr *na = st->get_attr (normattrname);
            TextIterator *it = na->textat (0);
            for (NumOfPos i = 0; i < st->size(); i++)
                norms[i] = atoi (it->next());
            delete it;
        } else {
            for (NumOfPos i = 0; i < st->size(); i++)
                norms[i] = st->rng->end_at(i) - st->rng->beg_at(i);
        }

        // save norms
        CorpInfo::VSC::const_iterator sattr = st->conf->attrs.begin();
        for (; sattr != st->conf->attrs.end(); ++sattr) {
            const string &aname = sattr->first;
            if (aname == normattrname)
                continue;
            cerr << "saving norms for " << aname << " ..." << endl;
            ToFile<int64_t> normf (st->conf->opts["PATH"] + aname + ".norm");
            PosAttr *at = st->get_attr (aname);
            int id, id_range = at->id_range();
            for (id = 0; id < id_range; id++) {
                NumOfPos ntotal = 0;
                FastStream *pos = at->id2poss (id);
                Position p, finval = pos->final();
                while ((p=pos->next()) < finval)
                    ntotal += norms[p];
                delete pos;
                normf.put (ntotal);
            }
        }
    } catch (exception &e) {
        cerr << "mknorms error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
