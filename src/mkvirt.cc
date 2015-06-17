// Copyright (c) 2011-2014  Pavel Rychly

#include <finlib/writelex.hh>
#include <finlib/log.hh>
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
        if (*newidf != -1)
            org[*newidf] = id;

    ToFile<int32_t> orgidf (target + ".oid");
    for (int id=0; id < lexsize; id++)
        orgidf(org[id]);
}

void make_full_lex (VirtualCorpus *vcorp, const string& attrname,
                    const string& target)
{
    CERR << "lexicon:" << attrname << ", " << target << '\n';
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

    write_lexicon wl (target, 500000);
    for (size_t i = 1; i < vcorp->segs.size(); i++) {
        CERR << "lexicon: seg:" << i << '\n';
        a = vcorp->segs[i].corp->get_attr(attrname);

        ToFile<int32_t> newid (target + n2str(i, ".nid"));
        int id, id_range = a->id_range();
        CERR << "lexicon: adding ids:" << id_range << '\n';
        for (id = 0; id < id_range; id++)
            newid (wl.str2id (a->id2str (id)));
    }

    ToFile<int64_t> freqs (target + ".frq");
    freqs(0);

    for (size_t i = 0; i < vcorp->segs.size(); i++)
        compute_orgid (target + n2str(i, ""), wl.size());
}

void prune_lex (PosAttr *pa, const string& target)
{
    CERR << "lexicon: pruning lexicon\n";
    const string tmptarget = target + ".tmp";
    {
    int id_range = pa->id_range();
    write_lexicon new_lex (tmptarget, min (500000, id_range));
    ToFile<int64_t> freqs (tmptarget + ".frq");
    for (int i = 0; i < id_range; i++) {
        FastStream *s = pa->id2poss(i);
        Position fin = s->final();
        NumOfPos frq = 0;
        while (s->next() < fin)
            frq++;
        delete s;
        if (frq) {
            new_lex.str2id (pa->id2str (i));
            freqs (frq);
        }
    }
    } // new_lex, freqs
    rename ((tmptarget + ".lex").c_str(), (target + ".lex").c_str());
    rename ((tmptarget + ".lex.idx").c_str(), (target + ".lex.idx").c_str());
    rename ((tmptarget + ".lex.srt").c_str(), (target + ".lex.srt").c_str());
    rename ((tmptarget + ".frq").c_str(), (target + ".frq").c_str());
}

void recompute_mappings (VirtualCorpus *vcorp, const string& attr,
                       const string& target)
{
    map_lexicon wl (target);
    for (size_t i = 0; i < vcorp->segs.size(); i++) {
        CERR << "lexicon: mapping:" << i << '\n';
        PosAttr *a = vcorp->segs[i].corp->get_attr (attr);
        {
        ToFile<int32_t> newid (target + n2str(i, ".nid"));
        for (int id = 0; id < a->id_range(); id++)
            newid (wl.str2id (a->id2str (id)));
        } // newid
        compute_orgid (target + n2str(i, ""), wl.size());
    }
}

void usage()
{
    cerr << "Usage: mkvirt [-d] [-a ATTRLIST] CORPUS\n"
         << "Options:\n"
         << "-d           skip creating dynamic attributes\n"
         << "-a ATTRLIST  compile only attributes in comma delimited\n"
         << "             ATTRLIST, may contain <struct>.<attr> attributes\n";
}

int main (int argc, char **argv) 
{
    string attrlist, structattrlist;
    bool skip_dynamic = false;
    int c;
    while ((c = getopt (argc, argv, "?hda:")) != -1)
        switch (c) {
        case 'd':
            skip_dynamic = true;
            break;
        case 'a':
            {
            istringstream as (optarg);
            string attrname;
            while (getline (as, attrname, ',')) {
                if (strchr (attrname.c_str(), '.'))
                    structattrlist += attrname + ",";
                else
                    attrlist += attrname + ",";
            }
            if (!attrlist.empty())
                attrlist.erase(attrlist.length() - 1);
            if (!structattrlist.empty())
                structattrlist.erase(structattrlist.length() - 1);
            }
            break;
        case 'h':
        case '?':
            usage();
            return 2;
        default:
            cerr << "mkvirt: unknown option (" << c << ")\n";
            usage();
            return 2;
        }

    const char *corpname;
    if (optind < argc) {
        corpname = argv [optind];
    } else {
        usage();
        return 2;
    }

    try {
        Corpus corp(corpname);
        if (attrlist.empty() && structattrlist.empty()) {
            attrlist = corp.get_conf("ATTRLIST");
            structattrlist = corp.get_conf("STRUCTATTRLIST");
        }
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
                CERR << "mkvirt: cannot make directory `" << corp_path << "'\n";
                return 1;
            }
        }
        corp_path += '/';

        istringstream as (attrlist);
        string attrname;
        while (getline (as, attrname, ',')) {
            if (corp.get_conf(attrname+".DYNAMIC") != "")
                continue;
            const string path = corp_path + attrname;
            make_full_lex(vcorp, attrname, path);
            prune_lex (corp.get_attr (attrname), path);
            recompute_mappings (vcorp, attrname, path);
        }
 
        istringstream ss (structattrlist);
        while (getline (ss, attrname, ',')) {
            if (corp.get_conf(attrname+".DYNAMIC") != "")
                continue;

            string strucname (attrname, 0, attrname.find ('.'));
            string atname (attrname, attrname.find ('.') +1);
            VirtualCorpus *vscorp = virtcorp2virtstruc (vcorp, strucname);

            const string path = corp_path + attrname;
            make_full_lex (vscorp, atname, path);
            Structure *struc = corp.get_struct (strucname);
            prune_lex (struc->get_attr (atname), path);
            recompute_mappings (vcorp, attrname, path);
            delete vscorp;
        }
        // create dynamic attributes
        if (!skip_dynamic) {
            stringstream ds;
            ds << attrlist << "," << structattrlist;
            while (getline (ds, attrname, ',')) {
                if (attrname.empty() || corp.get_conf(attrname+".DYNAMIC") == "")
                    continue;
                CERR << "Creating dynamic attribute " << attrname << endl;
                stringstream cmd;
                cmd << "mkdynattr " << corpname << " " << attrname;
                if (system (cmd.str().c_str()) != 0)
                    CERR << "ERROR: failed to create dynamic attribute "
                         << attrname << "\n";
            }
        }
    } catch (exception &e) {
        CERR << "mkvirt error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

