// Copyright (c) 2011-2013  Pavel Rychly

#include <finlib/writelex.hh>
#include "virtcorp.hh"
#include "corpus.hh"
#include <sstream>
#include <fstream>
#include <iostream>
#include <ios>


using namespace std;


void copy_file (const string& fromn, const string& ton)
{
    ifstream infile(fromn.c_str(), ios_base::binary);
    ofstream outfile(ton.c_str(), ios_base::binary);
    outfile << infile.rdbuf();
}

const char* n2str (int num, const char *suffix)
{
    static char str[16];
    sprintf (str, ".seg%i%s", num, suffix);
    return str;
}


void compute_orgid (const string& target, int lexsize)
{
    vector<int> org (lexsize, -1);
    FromFile<int32_t> newidf (target + ".nid");
    for (int id=0; !newidf; ++newidf, id++) 
        org[*newidf] = id;

    ToFile<int32_t> orgidf (target + ".oid");
    for (int id=0; id < lexsize; id++)
        orgidf(org[id]);
}

void make_full_lex (VirtualCorpus *vcorp, const string& attrname,
                    const string& target)
{
    cerr << "make_full_lex:" << attrname << ", " << target << '\n';
    PosAttr *a = vcorp->segs[0].corp->get_attr(attrname);
    copy_file (a->attr_path + ".lex", target + ".lex");
    copy_file (a->attr_path + ".lex.idx", target + ".lex.idx");
    copy_file (a->attr_path + ".lex.srt", target + ".lex.srt");
    {
        ToFile<int32_t> newid (target + n2str(0, ".nid"));
        int id, id_range = a->id_range();
        for (id = 0; id < id_range; id++)
            newid(id);
        // closing newid -- .seg0.nid
    }
    if (vcorp->segs.size() == 1)
        return;

    write_lexicon wl (target, 500000);
    for (size_t i = 1; i < vcorp->segs.size(); i++) {
        cerr << "make_full_lex: seg:" << i << '\n';
        a = vcorp->segs[i].corp->get_attr(attrname);

        ToFile<int32_t> newid (target + n2str(i, ".nid"));
        int id, nid, id_range = a->id_range();
        cerr << "make_full_lex: adding ids:" << id_range << '\n';
        for (id = 0; id < id_range; id++) {
            nid = wl.str2id (a->id2str (id));
            newid (nid);
        }
    }

    ToFile<int64_t> freqs (target + ".frq");
    int id, id_range = wl.size();
    for (id = 0; id < id_range; id++)
        freqs(0);

    for (size_t i = 0; i < vcorp->segs.size(); i++)
        compute_orgid (target + n2str(i, ""), wl.size());
}


void compute_freqs (VirtualCorpus *vcorp, const string& attrname,
                const string& target)
{
    cerr << "make_full_lex: computing freqs\n";
    PosAttr* a = setup_virtposattr (vcorp, target, attrname);
    ToFile<int64_t> freqs (target + ".frq");
    int id, id_range = a->id_range();
    for (id = 0; id < id_range; id++) {
        FastStream *s = a->id2poss(id);
        Position fin = s->final();
        int frq = 0;
        while (s->next() < fin)
            frq++;
        freqs(frq);
        delete s;
    }
}

void prune_lex (VirtualCorpus *vcorp, const string& attrname,
                const string& target)
{
    //XXX delete zero-freq from lexicon
}

int main (int argc, char **argv) 
{
    if (argc < 2) {
        fputs ("usage: mkvirt CORPUS\n", stderr);
        return 1;
    }
    try {
        Corpus corp(argv[1]);
        VirtualCorpus *vcorp = setup_virtcorp (corp.get_conf("VIRTUAL"));
        if (vcorp->segs.empty())
            return 2;

        string corp_path = corp.get_conf("PATH");
        if (*(corp_path.end() -1) == '/') // for windows
            corp_path.erase (corp_path.length() -1);
        struct stat dir_st;
        if (stat (corp_path.c_str(), &dir_st) < 0) {
        #if defined __MINGW32__
            #define mkdir_args
        #else
            #define mkdir_args , S_IRWXU | S_IRWXG | S_IXOTH | S_IROTH
        #endif
            if (mkdir (corp_path.c_str() mkdir_args)) {
                cerr << "mkvirt: cannot make directory `" << corp_path << "'\n";
                return 1;
            }
        }
        corp_path += '/';

        istringstream as (corp.get_conf("ATTRLIST"));
        string attrname;
        while (getline (as, attrname, ',')) {
            if (corp.get_conf(attrname+".DYNAMIC") != "")
                continue;
            make_full_lex(vcorp, attrname, corp_path + attrname);
            compute_freqs (vcorp, attrname, corp_path + attrname);
            // XXX prune_lex (vcorp, attrname, corp_path + attrname);
        }
 
        istringstream ss (corp.get_conf("STRUCTATTRLIST"));
        while (getline (ss, attrname, ',')) {
            if (corp.get_conf(attrname+".DYNAMIC") != "")
                continue;

            string strucname (attrname, 0, attrname.find ('.'));
            string atname (attrname, attrname.find ('.') +1);
            VirtualCorpus *vscorp = virtcorp2virtstruc (vcorp, strucname);

            make_full_lex (vscorp, atname, corp_path + attrname);
            compute_freqs (vscorp, atname, corp_path + attrname);
            // XXX prune_lex (vscorp, attrname, corp_path + attrname);
            delete vscorp;
        }
    } catch (exception &e) {
        cerr << "mkvirt error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

