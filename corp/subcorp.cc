//  Copyright (c) 2002-2015  Pavel Rychly, Milos Jakubicek

#include "subcorp.hh"
#include "frsop.hh"
#include "cqpeval.hh"
#include <finlib/fromtof.hh>
#include <dirent.h>
#include <iostream>
#include <sys/stat.h>
#include <stdint.h>

using namespace std;

class FilterIDPosIterator: public IDPosIterator {
    IDPosIterator *it;
    RangeStream *rs;
    NumOfPos delta;
    bool is_end;
public:
    FilterIDPosIterator (IDPosIterator *it, RangeStream *rs):
        it (it), rs (rs), delta (rs->peek_beg()), is_end (false) {locate();}
    ~FilterIDPosIterator() {delete it; delete rs;}
    void next() {it->next(); locate();}
    Position peek_pos() {return it->peek_pos();}
    NumOfPos get_delta() {return delta;}
    int peek_id() {return it->peek_id();}
    bool end() {return is_end || it->end();}
protected:
    void locate() {
        if (it->end())
            return;
        while (it->peek_pos() >= rs->peek_end() && !rs->end()) {
            Position last_end = rs->peek_end();
            rs->next();
            delta += rs->peek_beg() - last_end;
        }
        if (rs->end()) {
            is_end = true;
            return;
        }
        while (it->peek_pos() < rs->peek_beg() && !it->end())
            it->next();
    }
};

SubCorpus::SubCorpus(const Corpus *corp, const string &sub, bool complement)
    : Corpus (corp->conf), search_size_cached (0),
      subcorp (create_ranges (sub, "map64")), complement (complement)
{
    conf->opts["SUBCPATH"] = sub;
    size_t dot = conf->opts["SUBCPATH"].find_last_of(".");
    if (dot != string::npos && dot != conf->opts["SUBCPATH"].size() - 1)
        conf->opts["SUBCPATH"].erase(dot + 1, string::npos);
}

SubCorpus::~SubCorpus ()
{
    delete subcorp;
}

RangeStream *SubCorpus::filter_query(RangeStream *s)
{
    RangeStream *subc = subcorp->whole();
    if (complement)
        subc = RQoutsideNode::create (subc, search_size());
    return new RQinNode (s, subc);
}

IDPosIterator *SubCorpus::filter_idpos (IDPosIterator *it)
{
    return new FilterIDPosIterator (it, subcorp->whole());
}

NumOfPos SubCorpus::search_size()
{
    if (!search_size_cached) {
        RangeStream *s = subcorp->whole();
        NumOfPos sum = 0;
        do {
            sum += s->peek_end() - s->peek_beg();
        } while (s->next());
        if (complement)
            search_size_cached = size() - sum;
        else
            search_size_cached = sum;
    }
    return search_size_cached;
}

string SubCorpus::get_info()
{
    return "";
}

static inline bool isDir (const char *path)
{
    struct stat st;
    if (stat (path, &st))
        return false;
    return S_ISDIR (st.st_mode);
}

bool create_subcorpus (const char *subcpath, RangeStream *r, Structure *struc)
{
    if (struc)
        r = new RQcontainNode (struc->rng->whole(), r);
    if (r->end()) {
        delete r;
        return false;
    }
    ToFile<int64_t> outf (subcpath);
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
    return true;
}

bool create_subcorpus (const char *subcpath, Corpus *c,
                       const char *structname, const char *query)
{
    Structure *struc = c->get_struct (structname);
    FastStream *fs = eval_cqponepos ((string (query) + ';').c_str(), struc);
    return create_subcorpus (subcpath, c->filter_query (struc->rng->part (fs)));
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

// -------------------SubCorpPosAttr -----------------------

#define GET_FREQ(freqf, func) \
    if (freqf) { \
        NumOfPos freq = (*(freqf))[id]; \
        if (complement) \
            return attr->func (id) - freq; \
        return freq; \
    } else \
        return -1LL;

template <class NormClass=MapBinFile<int64_t>,
          class FreqClass=MapBinFile<uint32_t>,
          class FloatFreqClass=MapBinFile<float> >
class SubCorpPosAttr : public PosAttr
{
    PosAttr *attr;
    NormClass *normf, *freq64f;
    FreqClass *freqf, *docff;
    FloatFreqClass *arff, *aldff;
    bool complement;
public:
    SubCorpPosAttr (PosAttr *pa, const string &subcpath, bool complement)
        : PosAttr (pa->attr_path, pa->name, pa->locale, pa->encoding),
          attr (pa), normf (NULL), freq64f (NULL), freqf (NULL), docff (NULL),
          arff (NULL), aldff (NULL), complement (complement)
    {
        const string path = subcpath + pa->name;
        try {
            freqf = new FreqClass (path + ".frq");
        } catch (FileAccessError) {
            try {
                freq64f = new NormClass (path + ".frq64");
            } catch (FileAccessError) {}
        }
        try {
            normf = new NormClass (path + ".norm");
        } catch (FileAccessError) {}
        try {
            docff = new FreqClass (path + ".docf");
        } catch (FileAccessError) {}
        try {
            arff = new FloatFreqClass (path + ".arf");
        } catch (FileAccessError) {}
        try {
            aldff = new FloatFreqClass (path + ".aldf");
        } catch (FileAccessError) {}
    }
    virtual ~SubCorpPosAttr () {
        delete attr;
        delete freqf; delete freq64f;
        delete normf; delete docff; delete arff; delete aldff;
    }
    virtual int id_range () {return attr->id_range();}
    virtual const char* id2str (int id) {return attr->id2str (id);}
    virtual int str2id (const char *str) {return attr->str2id (str);}
    virtual int pos2id (Position pos) {return attr->pos2id (pos);}
    virtual const char* pos2str (Position pos) {return attr->pos2str (pos);}
    virtual IDIterator *posat (Position pos) {return attr->posat (pos);}
    virtual IDPosIterator *idposat (Position pos) {return attr->idposat (pos);}
    virtual TextIterator *textat (Position pos) {return attr->textat (pos);}
    virtual FastStream *id2poss (int id) {return attr->id2poss(id);}
    virtual FastStream *regexp2poss (const char *pat, bool ignorecase)
            {return attr->regexp2poss (pat, ignorecase);}
    virtual FastStream *compare2poss (const char *pat, int cmp, bool ignorecase)
            {return attr->compare2poss (pat, cmp, ignorecase);}
    virtual Generator<int> *regexp2ids (const char *pat, bool ignorecase,
                                        const char *filter_pat = NULL)
            {return attr->regexp2ids (pat, ignorecase, filter_pat);}
    virtual NumOfPos size() {return attr->size();}
    // non-forwarding methods follow
    virtual NumOfPos freq (int id) {if (freqf) {GET_FREQ (freqf, freq)}
                                    else {GET_FREQ (freq64f, freq)}}
    virtual NumOfPos docf (int id) {GET_FREQ (docff, docf)}
    virtual float arf (int id) {GET_FREQ (arff, arf)}
    virtual float aldf (int id) {GET_FREQ (aldff, aldf)}
    virtual NumOfPos norm (int id) {if (normf) {GET_FREQ (normf, norm)}
                                    else return freq (id);}
};

PosAttr * createSubCorpPosAttr (PosAttr *pa, const string &subcpath,
                                bool complement) {
    return new SubCorpPosAttr<> (pa, subcpath, complement);
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
