//  Copyright (c) 2000-2011 Pavel Rychly

#include <finlib/fsop.hh>
#include <finlib/lexicon.hh>
#include <finlib/revidx.hh>
#include <finlib/regexplex.hh>
#include "dynattr.hh"
#include <cstdlib>

using namespace std;

class DummyIDIter: public IDIterator {
public:
    DummyIDIter () {}
    virtual int next() {return 0;}
};


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
    DynAttr (DynFun *fn, PosAttr *attr, const string &attrpath = "", 
             const string &n = "", const string &loc="", const string &enc="",
             bool ownedByCorpus = true)
        : PosAttr (attrpath, n, loc == "" ? attr->locale : loc, 
                   attr->encoding), 
          rattr (attr), fun(fn), ownedByCorpus(ownedByCorpus) {}
    virtual ~DynAttr () {
        delete fun;
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
    virtual NumOfPos norm (int id) {return 0;}
    virtual NumOfPos size () {return rattr->size();}
};

class DynAttr_withLex: public DynAttr {
protected:
    bool transquery;
public:
    class IDIter: public IDIterator {
        TextIterator *it;
        DynAttr_withLex &at;
    public:
        IDIter (TextIterator *i, DynAttr_withLex &a): it(i), at(a) {}
        virtual int next() {return at.lex.str2id ((*at.fun) (it->next()));}
        virtual ~IDIter() {delete it;}
    };
    map_lexicon lex;
    DynAttr_withLex (DynFun *fn, PosAttr *attr, const string &attrpath,
                     const string &n, const string &loc="", bool transq=false,
                     bool ownedByCorpus = true)
        : DynAttr (fn, attr, attrpath, n, loc, "", ownedByCorpus),
          transquery (transq), lex (attrpath) {}
    virtual int id_range () {return lex.size();}
    virtual const char* id2str (int id) {return lex.id2str (id);}
    virtual int str2id(const char *str) 
        {return lex.str2id (transquery ? (*fun)(str) : str);}
    virtual int pos2id (Position pos) 
        {return lex.str2id ((*fun) (rattr->pos2str (pos)));}
    virtual const char* pos2str (Position pos) 
        {return (*fun) (rattr->pos2str (pos));}
    virtual IDIterator * posat (Position pos) 
        {return new IDIter (rattr->textat (pos), *this);}
    virtual Generator<int> *regexp2ids (const char *pat, bool ignorecase, const char *filter_pat)
        {return ::regexp2ids (lex, pat, locale, encoding, ignorecase, filter_pat);}
 
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
          rev (attrpath) {}
    virtual FastStream *id2poss (int id)
        {return ID_list2poss (rev.id2poss (id));}
    virtual FastStream *regexp2poss (const char *pat, bool ignorecase)
        {return ID_list2poss (::regexp2poss (rev, lex, 
                                             transquery ? (*fun)(pat) : pat,
                                             locale,
                                             encoding,
                                             ignorecase));}
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


PosAttr *createDynAttr (const string &type, const string &apath, 
                        const string &n, DynFun *fun, PosAttr *from, 
                        const string &locale, bool trasquery,
                        bool ownedByCorpus)
{
    if (type == "default" || type == "plain")
        return new DynAttr (fun, from, apath, n, locale, "", ownedByCorpus);
    else if (type == "lexicon")
        return new DynAttr_withLex (fun, from, apath, n, locale, trasquery,
                                    ownedByCorpus);
    else if (type == "index")
        return new DynAttr_withIndex (fun, from, apath, n, locale, trasquery,
                                      ownedByCorpus);
    else
        throw AttrNotFound ("Dynamic (" + type + "):" + apath);
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

