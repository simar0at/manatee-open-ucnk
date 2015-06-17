//  Copyright (c) 1999-2015  Pavel Rychly, Milos Jakubicek

#include "posattr.hh"
#include "dynattr.hh"
#include "regexopt.hh"
#include <finlib/fsop.hh>
#include <finlib/lexicon.hh>
#include <finlib/regexplex.hh>

class UniqPosAttr : public PosAttr
{
public:
    class IDIter: public IDIterator {
        SequenceStream it;
    public:
        IDIter (Position first, Position last): it(first, last, -1) {}
        virtual int next() {return it.next();}
        virtual ~IDIter() {}
    };
    class TextIter: public TextIterator {
        SequenceStream it;
        map_lexicon &lex;
    public:
        TextIter (Position first, map_lexicon &l): 
            it(first, l.size() -1, -1), lex(l) {}
        virtual const char *next() {return lex.id2str (it.next());}
        virtual ~TextIter() {}
    };
    map_lexicon lex;
    MapBinFile<int64_t> *normf;
    PosAttr *regex;
    
    UniqPosAttr (const std::string &path, const std::string &n, const std::string &locale, 
                 const std::string &encoding, int text_size)
        :PosAttr (path, n, locale, encoding), lex (path), normf (NULL),
         regex (NULL)
    {
        try {
            normf = new  MapBinFile<int64_t> (path + ".norm");
        } catch (FileAccessError) {}
        try {
            DynFun *fun = createDynFun ("", "internal", "lowercase"); // lowercase = dummy here
            regex = createDynAttr ("index", path + ".regex", n + ".regex", fun,
                                   this, locale, false);
        } catch (FileAccessError) {}
    }
    virtual ~UniqPosAttr () {delete normf; delete regex;}

    virtual int id_range () {return lex.size();}
    virtual const char* id2str (int id) {return lex.id2str (id);}
    virtual int str2id(const char *str) {return lex.str2id (str);}
    virtual int pos2id (Position pos) {return pos;}
    virtual const char* pos2str (Position pos) {return lex.id2str (pos);}
    virtual IDIterator *posat (Position pos)
        {return new IDIter (pos, lex.size());}
    virtual IDPosIterator *idposat (Position pos)
        {return new IDPosIterator (new IDIter (pos, lex.size()),
                                   new SequenceStream (0, size()-1, size()));}
    virtual TextIterator *textat (Position pos) {return new TextIter (pos, lex);}
    virtual FastStream *id2poss (int id) 
        {return new SequenceStream (id, id, lex.size());}
    virtual FastStream *compare2poss (const char *pat, int cmp, bool ignorecase) {
        return ::compare2poss<UniqPosAttr, map_lexicon>
            (*this, lex, pat, cmp, ignorecase);
    }
    virtual FastStream *regexp2poss (const char *pat, bool ignorecase) {
        FastStream *fs = NULL;
        if (regex)
            fs = optimize_regex (regex, pat, encoding);
        return new Gen2Fast<int>(
            ::regexp2ids<map_lexicon>(lex, pat, locale, encoding, ignorecase,
                                      NULL, fs));
    }
    virtual Generator<int> *regexp2ids (const char *pat, bool ignorecase,
                                        const char *filter_pat) {
        FastStream *fs = NULL;
        if (regex)
            fs = optimize_regex (regex, pat, encoding);
        return ::regexp2ids<map_lexicon> (lex, pat, locale, encoding,
                                          ignorecase, filter_pat, fs);
    }
    virtual NumOfPos freq (int id) {return 1;}
    virtual NumOfPos docf (int id) {return 1;}
    virtual float arf (int id) {return 1;}
    virtual float aldf (int id) {return 1;}
    virtual NumOfPos norm (int id) {return normf ? (*normf)[id] : 1;}
    virtual NumOfPos size () {return lex.size();}
};

PosAttr *createUniqPosAttr (const std::string &path, const std::string &n,
                            const std::string &locale, const std::string &enc,
                            int text_size = 0) {
    return new UniqPosAttr (path, n, locale, enc, text_size);
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
