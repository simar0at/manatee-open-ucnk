//  Copyright (c) 2000-2015  Pavel Rychly, Milos Jakubicek

#include <finlib/fsop.hh>
#include <finlib/lexicon.hh>
#include <finlib/revidx.hh>
#include <finlib/regexplex.hh>
#include "dynattr.hh"
#include "regexopt.hh"
#include <cstdlib>

using namespace std;

class DummyIDIter: public IDIterator {
public:
    DummyIDIter () {}
    virtual int next() {return 0;}
};


template <class FreqClass=MapBinFile<uint32_t>,
          class FloatFreqClass=MapBinFile<float> >
class DynAttr: public PosAttr {
public:
    class TextIter: public TextIterator {
        TextIterator *it;
        DynFun *fun;
    public:
        TextIter (TextIterator *i, DynFun *f): it(i), fun(f) {}
        virtual const char *next() {return (*fun)(it->next());}
        virtual ~TextIter() {delete it;}
    };
    PosAttr *rattr;
    DynFun *fun;
    bool ownedByCorpus;
    FreqClass *docff;
    FloatFreqClass *arff, *aldff;
    DynAttr (DynFun *fn, PosAttr *attr, const string &attrpath = "", 
             const string &n = "", const string &loc="", const string &enc="",
             bool ownedByCorpus = true)
        : PosAttr (attrpath, n, loc == "" ? attr->locale : loc, 
                   attr->encoding), 
          rattr (attr), fun(fn), ownedByCorpus(ownedByCorpus),
          docff (NULL), arff (NULL), aldff (NULL)
    {
        try {
            docff = new FreqClass (attrpath + ".docf");
        } catch (FileAccessError) {}
        try {
            arff = new FloatFreqClass (attrpath + ".arf");
        } catch (FileAccessError) {}
        try {
            aldff = new FloatFreqClass (attrpath + ".aldf");
        } catch (FileAccessError) {}
    }
    virtual ~DynAttr () {
        delete fun;
        delete docff;
        delete arff;
        delete aldff;
        if (!ownedByCorpus) {
            delete rattr;
        }
    }
    virtual int id_range () {return 0;}
    virtual const char* id2str (int id) {return "";}
    virtual int str2id(const char *str) {return 0;}
    virtual int pos2id (Position pos) {return 0;}
    virtual const char* pos2str (Position pos) 
        {return (*fun) (rattr->pos2str (pos));}
    virtual IDIterator * posat(Position pos) 
        {return new DummyIDIter ();}
    virtual IDPosIterator * idposat(Position pos)
        {return new IDPosIterator (new DummyIDIter (), 0);}
    virtual TextIterator * textat(Position pos) 
        {return new TextIter (rattr->textat (pos), fun);}
    virtual FastStream *id2poss (int id)
        {return new EmptyStream();}
    virtual FastStream *regexp2poss (const char *pat, bool)
        {return new EmptyStream();}
    virtual FastStream *compare2poss (const char *pat, int, bool)
        {return new EmptyStream();}
    virtual Generator<int> *regexp2ids (const char *pat, bool ignorecase, const char *filter_pat)
        {return new EmptyGenerator<int>();}
    virtual NumOfPos freq (int id) {return 0;}
    virtual NumOfPos docf (int id) {return docff ? (*docff)[id] : -1LL;}
    virtual float arf (int id) {return arff ? (*arff)[id] : -1LL;}
    virtual float aldf (int id) {return aldff ? (*aldff)[id] : -1LL;}
    virtual NumOfPos norm (int id) {return 0;}
    virtual NumOfPos size () {return rattr->size();}
};

class DynAttr_withLex: public DynAttr<> {
protected:
    bool transquery;
public:
    class IDIter: public IDIterator {
        TextIterator *ti;
        IDIterator *ii;
        DynAttr_withLex &at;
        Position pos, final;
    public:
        IDIter (DynAttr_withLex &a, Position p):
            ti(NULL), ii(NULL), at(a), pos(p), final(a.rattr->size()) {
            if (a.ridx)
                ii = a.rattr->posat (p);
            else
                ti = a.rattr->textat (p);
        }
        virtual int next() {
            if (pos >= 0 && pos++ < final) {
                if (at.ridx)
                    return (*(at.ridx))[ii->next()];
                else
                    return at.lex.str2id ((*at.fun) (ti->next()));
            } else
               return -1;
        }
        virtual ~IDIter() {delete ti; delete ii;}
    };
    class TextIter: public TextIterator {
        IDIterator *it;
        DynAttr_withLex &at;
    public:
        TextIter (IDIterator *i, DynAttr_withLex &a): it(i), at(a) {}
        virtual const char *next() {return at.lex.id2str((*(at.ridx))[it->next()]);}
        virtual ~TextIter() {delete it;}
    };
    map_lexicon lex;
    MapBinFile<lexpos> *ridx;
    PosAttr *regex;

    DynAttr_withLex (DynFun *fn, PosAttr *attr, const string &attrpath,
                     const string &n, const string &loc="", bool transq=false,
                     bool ownedByCorpus = true)
        : DynAttr<> (fn, attr, attrpath, n, loc, "", ownedByCorpus),
          transquery (transq), lex (attrpath), ridx (NULL), regex (NULL)
    {
        try {
            ridx = new MapBinFile<lexpos> (attrpath + ".lex.ridx");
        } catch (FileAccessError) {}
        size_t len = n.size();
        if (len < 7 || strcmp (n.c_str() + len - 6, ".regex")) {
            try {
            DynFun *fun = createDynFun ("", "internal", "lowercase"); // lowercase = dummy here
            regex = createDynAttr ("index", attrpath + ".regex", n + ".regex",
                                   fun, this, loc, false);
            } catch (FileAccessError) {}
        }

    }
    ~DynAttr_withLex() {delete ridx; delete regex;}
    virtual int id_range () {return lex.size();}
    virtual const char* id2str (int id) {return lex.id2str (id);}
    virtual int str2id(const char *str) 
        {return lex.str2id (transquery ? (*fun)(str) : str);}
    virtual int pos2id (Position pos) 
        {return ridx ? (*ridx)[rattr->pos2id (pos)]
                     : lex.str2id ((*fun) (rattr->pos2str (pos)));}
    virtual const char* pos2str (Position pos) 
        {return ridx ? lex.id2str((*ridx)[rattr->pos2id (pos)])
                     : (*fun) (rattr->pos2str (pos));}
    virtual IDIterator * posat (Position pos) 
        {return new IDIter (*this, pos);}
    virtual IDPosIterator * idposat (Position pos)
        {return new IDPosIterator (new IDIter (*this, pos),
                                   new SequenceStream (pos, size()-1, size()));}
    virtual TextIterator * textat(Position pos)
        {return ridx ? (TextIterator*) new TextIter (rattr->posat (pos), *this)
           : (TextIterator*) new DynAttr<>::TextIter (rattr->textat (pos), fun);}
    virtual Generator<int> *regexp2ids (const char *pat, bool ignorecase, const char *filter_pat)
        {
            if (regex) {
                FastStream *fs = optimize_regex (regex, pat, encoding);
                return ::regexp2ids (lex, pat, locale, encoding, ignorecase,
                                     filter_pat, fs);
            }
            return ::regexp2ids (lex, pat, locale, encoding, ignorecase, filter_pat);
        }
 
};

class DynAttr_withIndex: public DynAttr_withLex {
protected:
    FastStream *ID_list2poss (FastStream *ids);
public:
    map_delta_revidx rev;
    DynAttr_withIndex (DynFun *fn, PosAttr *attr, const string &attrpath,
                       const string &n, const string &loc="", 
                       bool transq=false, bool ownedByCorpus = true)
        : DynAttr_withLex (fn, attr, attrpath, n, loc, transq, ownedByCorpus),
          rev (attrpath, rattr->id_range()) {}
    virtual FastStream *id2poss (int id)
        {return ID_list2poss (rev.id2poss (id));}
    virtual FastStream *dynid2srcids (int id) {return rev.id2poss (id);}
    virtual FastStream *regexp2poss (const char *pat, bool ignorecase)
    {
        FastStream *fs = NULL;
        if (regex)
            fs = optimize_regex (regex, pat, encoding);
        return ID_list2poss (::regexp2poss (rev, lex,
                                            transquery ? (*fun)(pat) : pat,
                                            locale, encoding, ignorecase, fs));
    }
    virtual NumOfPos freq (int id);
    virtual NumOfPos norm (int id);
};

FastStream *DynAttr_withIndex::ID_list2poss (FastStream *ids)
{
    std::vector<FastStream*> *fsv = new std::vector<FastStream*>;
    fsv->reserve(10);
    while (ids->peek() < ids->final()) {
        int id = ids->next();
        FastStream *s = rattr->id2poss (id);
        fsv->push_back(s);
    }
    delete ids;
    if (fsv->size())
        return new QOrVNode (fsv);
    else {
        delete fsv;
        return new EmptyStream();
    }
}

NumOfPos DynAttr_withIndex::freq (int id) {
    NumOfPos count = 0;
    FastStream *fs = rev.id2poss (id);
    while (fs->peek() < fs->final())
        count += rattr->freq (fs->next());
    delete fs;
    return count;
}

NumOfPos DynAttr_withIndex::norm (int id) {
    NumOfPos count = 0;
    FastStream *fs = rev.id2poss (id);
    while (fs->peek() < fs->final())
        count += rattr->norm (fs->next());
    delete fs;
    return count;
}

template <class NormClass=MapBinFile<int64_t> >
class DynAttr_withFreq: public DynAttr_withIndex {
    NormClass *normf, *freqf;
public:
    DynAttr_withFreq (DynFun *fn, PosAttr *attr, const string &attrpath,
                      const string &n, const string &loc="",
                      bool transq=false, bool ownedByCorpus = true)
        : DynAttr_withIndex (fn, attr, attrpath, n, loc, transq, ownedByCorpus),
          normf (NULL)
    {
        freqf = new NormClass (attrpath + ".freq");
        try {
            normf = new NormClass (attrpath + ".norm");
        } catch (FileAccessError) {}
    }
    virtual NumOfPos freq (int id) {return (*freqf)[id];}
    virtual NumOfPos norm (int id) {return normf ? (*normf)[id] : -1LL;}
};


PosAttr *createDynAttr (const string &type, const string &apath, 
                        const string &n, DynFun *fun, PosAttr *from, 
                        const string &locale, bool trasquery,
                        bool ownedByCorpus)
{
    if (type == "default" || type == "plain")
        return new DynAttr<> (fun, from, apath, n, locale, "", ownedByCorpus);
    else if (type == "lexicon")
        return new DynAttr_withLex (fun, from, apath, n, locale, trasquery,
                                    ownedByCorpus);
    else if (type == "index")
        return new DynAttr_withIndex (fun, from, apath, n, locale, trasquery,
                                      ownedByCorpus);
    else if (type == "freq") {
        return new DynAttr_withFreq<> (fun, from, apath, n, locale, trasquery,
                                       ownedByCorpus);
    }
    else
        throw AttrNotFound ("Dynamic (" + type + "):" + apath);
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

