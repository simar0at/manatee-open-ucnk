//  Copyright (c) 2002-2011  Pavel Rychly

#include "subcorp.hh"
#include "frsop.hh"
#include <finlib/fromtof.hh>
#include <dirent.h>
#include <iostream>
#include <sys/stat.h>
#include <stdint.h>

using namespace std;

SubCorpus::SubCorpus(const Corpus *corp, const string &sub)
    : Corpus (corp->conf), search_size_cached (0), 
      subcorp (create_ranges (sub, "map64"))
{
}

SubCorpus::~SubCorpus ()
{
    delete subcorp;
}

RangeStream *SubCorpus::filter_query(RangeStream *s)
{
    return new RQinNode (s, subcorp->whole());
}

NumOfPos SubCorpus::search_size()
{
    if (!search_size_cached) {
        RangeStream *s = subcorp->whole();
        NumOfPos sum = 0;
        do {
            sum += s->peek_end() - s->peek_beg();
        } while (s->next());
        search_size_cached = sum;
    }
    return search_size_cached;
}

string SubCorpus::get_info()
{
    return "";
}

NumOfPos SubCorpus::compute_docf(FastStream *poss, RangeStream *struc) {
    return Corpus::compute_docf(poss, filter_query(struc));
}

static inline bool isDir (const char *path)
{
    struct stat st;
    if (stat (path, &st))
        return false;
    return S_ISDIR (st.st_mode);
}

void save_subcorpus (const char *subcpath, FastStream *src, Corpus *corp,
                     ranges *struc)
{
    ToFile<int64_t> outf (subcpath);
    RangeStream *r = corp->filter_query (struc->part(src));
    if (r->end())
        return;
    outf (r->peek_beg());
    Position lastend = r->peek_end();
    while (r->next()) {
        Position p = r->peek_beg();
        if (lastend != p) {
            outf (lastend);
            outf (p);
        }
        lastend = r->peek_end();
    }
    outf(lastend);
    delete r;
}

SubCorpus *create_subcorpus (const char *subcdir, const string &corpname,
                             const string &subcname, FastStream *src, 
                             Corpus *corp, ranges *struc)
{
    string path = subcdir + ("/" + corpname);
    if (! isDir (path.c_str())) {
        if (mkdir (path.c_str()
#if ! defined __MINGW32__
                   , S_IRWXU |S_IRWXG
#endif
                   ))
            throw FileAccessError (path, "create_subc: mkdir");
    }
    path += "/" + subcname + ".subc";
    save_subcorpus (path.c_str(), src, corp, struc);
    return new SubCorpus (corp, path);
}

void find_subcorpora (const char *subcdir, 
                      map<string,pair<string,string> > &scs)
{
    DIR *dp;
    struct dirent *ep;
     
    dp = opendir (subcdir);
    if (dp == NULL) {
        cerr << "Couldn't open " << subcdir << " directory\n";
        return;
    }
    string base (subcdir);
    base += '/';
    while ((ep = readdir (dp))) {
        if (ep->d_name[0] != '.' && isDir ((base + ep->d_name).c_str())) {
            DIR *cdp = opendir ((base + ep->d_name).c_str());
            if (cdp == NULL) continue;
            string corpname = ep->d_name;
            string corppath = base + corpname + '/';
            struct dirent *cep;
            while ((cep = readdir (cdp))) {
                int len = strlen (cep->d_name);
                if (len < 6) continue;
                if (strcmp (".subc", cep->d_name + len -5)) continue;
                string sname = corpname + ':' + string (cep->d_name, len -5);
                scs [sname] = make_pair (corpname, corppath + cep->d_name);
            }
            closedir (cdp);
        }
    }
    closedir (dp);
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
