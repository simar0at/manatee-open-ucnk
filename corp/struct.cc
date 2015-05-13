//  Copyright (c) 1999-2013  Pavel Rychly

#include "corpus.hh"

using namespace std;

Structure::Structure (CorpInfo *i, const string &path, const string &n)
    : Corpus (i, Struct_type), 
      rng (create_ranges(path + ".rng", i->opts["TYPE"])), name (n), 
    endtagstring ("</" + name + '>')
{
}

Structure::Structure (CorpInfo *i, const string &n, VirtualCorpus *vc)
    : Corpus (i, Struct_type), rng (setup_virtstructrng (vc, n)), 
      name (n), endtagstring ("</" + n + '>')
{
    virt = virtcorp2virtstruc (vc, n);
}


class StructPosAttr : public PosAttr
{
    Structure *st;
    PosAttr *pa;
    Position lastrng;
    bool nested;
    char multisep;
    Position locate_rng (Position pos) {
        if (!nested && // no cache for nested structures
            st->rng->beg_at (lastrng) <= pos && st->rng->end_at (lastrng) > pos)
            return lastrng;
        else {
            Position n = st->rng->num_at_pos (pos);
            if (n >= 0)
                lastrng = n;
            return n;
        }
    }
public:
    StructPosAttr (Structure *st, PosAttr *pa)
        : PosAttr (pa->attr_path, (st->name + '.' + pa->name), pa->locale),
          st (st), pa(pa), lastrng (0), 
          nested (CorpInfo::str2bool(st->get_conf("NESTED"))),
          multisep (st->conf->find_attr(pa->name)["MULTISEP"][0])
    {}
    virtual ~StructPosAttr () {}
    virtual int id_range () {return pa->id_range();}
    virtual const char* id2str (int id) {return pa->id2str (id);}
    virtual int str2id(const char *str) {return pa->str2id (str);}
    virtual int pos2id (Position pos) {
        Position n = locate_rng (pos);
        return (n == -1) ? -1 : pa->pos2id (n);
    }
    virtual const char* pos2str (Position pos) {
        Position n = locate_rng (pos);
        if (n == -1)
            return "";
        if (!nested || !st->rng->nesting_at(n))
            return pa->pos2str (n);
        while (st->rng->nesting_at(n))
            n--;
        string ret = "";
        while ((st->rng->nesting_at(n) || pos >= st->rng->beg_at(n))
               && n < st->rng->size()) {
            if (pos >= st->rng->beg_at(n) && pos < st->rng->end_at(n)) {
                ret += pa->pos2str (n);
                ret += multisep;
            }
            n++;
        }
        if (!ret.empty())
            ret.replace (ret.length() -1, 1, "");
        return ret.c_str();
    }
    virtual IDIterator *posat (Position pos) {NOTIMPLEMENTED}
    virtual TextIterator *textat (Position pos) {NOTIMPLEMENTED}
    virtual FastStream *id2poss (int id) {NOTIMPLEMENTED}
    virtual FastStream *regexp2poss (const char *pat, bool) {NOTIMPLEMENTED}
    virtual FastStream *compare2poss (const char *pat, int, bool) {NOTIMPLEMENTED}
    virtual Generator<int> *regexp2ids (const char *pat, bool ignorecase, const char *filter_pat)
        {return pa->regexp2ids (pat,ignorecase,filter_pat);}
    virtual NumOfPos freq (int id) {return pa->freq (id);}
    virtual NumOfPos norm (int id) {return pa->norm (id);}
    virtual NumOfPos size () {NOTIMPLEMENTED}
};



PosAttr* Corpus::get_struct_pos_attr (const std::string &strname, 
                                      const std::string &attname)
{
    Structure* s = get_struct (strname);
    PosAttr* a = s->get_attr (attname);
    StructPosAttr *spa = new StructPosAttr (s, a);
    attrs.push_back (pair<string,PosAttr*> (strname + "." + attname, spa));
    return spa;
}


/*
XXX nesting musi byt ulozeno v .rng a tady se musi predavat
    pro nesting > 0 tam musi byt i final (max end)
RangeStream *Structure::whole()
{
    return new whole_range (&rng);
}


RangeStream *Structure::part(FastStream *filter)
{
    return new part_range (&rng, filter);
}

*/

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

