//  Copyright (c) 1999-2015  Pavel Rychly, Milos Jakubicek

#include <finlib/lexicon.hh>
#include <finlib/revidx.hh>
#include <finlib/text.hh>
#include <finlib/regexplex.hh>
#include "posattr.hh"
#include "pauniq.hh"
#include "dynattr.hh"
#include "regexopt.hh"
#include <set>

using namespace std;

//-------------------- PosAttr --------------------
const char *encoding2c_str (const string &encoding);

PosAttr::PosAttr(const string &path, const string &n, const string &loc,
                 const string &enc)
    :attr_path (path), name (n), locale (locale2c_str (loc)), 
     encoding (encoding2c_str (enc))
{
}

PosAttr::~PosAttr()
{
}

static set<string> locale_strings;

const char *locale2c_str (const string &locale)
{
    return (*locale_strings.insert (locale).first).c_str();
}
static set<string> encoding_strings;

const char *encoding2c_str (const string &encoding)
{
    return (*encoding_strings.insert (encoding).first).c_str();
}

//-------------------- GenPosAttr<RevClass,TextClass> --------------------

template <class RevClass, class TextClass, class LexClass, 
          class NormClass=MapBinFile<int64_t>,
          class FreqClass=MapBinFile<uint32_t>,
          class FloatFreqClass=MapBinFile<float> >
class GenPosAttr : public PosAttr
{
public:
    class IDIter: public IDIterator {
        typename TextClass::const_iterator it;
    public:
        IDIter (const typename TextClass::const_iterator &i): it(i) {}
        virtual int next() {return it.next();}
    };
    class TextIter: public TextIterator {
        typename TextClass::const_iterator it;
        LexClass &lex;
    public:
        TextIter (const typename TextClass::const_iterator &i, 
                   LexClass &l): it(i), lex(l) {}
        virtual const char *next() {return lex.id2str (it.next());}
    };
    LexClass lex;
    TextClass txt;
    RevClass rev;
    NormClass *normf;
    FreqClass *docff;
    FloatFreqClass *arff, *aldff;
    PosAttr *regex;
    
    GenPosAttr (const string &path, const string &n, const string &locale, 
                const string &encoding, NumOfPos text_size=0)
        :PosAttr (path, n, locale, encoding), lex (path), 
         txt (path, text_size), rev (path, txt.size()), normf (NULL),
         docff (NULL), arff (NULL), aldff (NULL), regex (NULL)
    {
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
        try {
            DynFun *fun = createDynFun ("", "internal", "lowercase"); // lowercase = dummy here
            regex = createDynAttr ("index", path + ".regex", n + ".regex", fun,
                                   this, locale, false);
        } catch (FileAccessError) {}
    }
    virtual ~GenPosAttr ()
        {delete normf; delete docff; delete arff; delete aldff; delete regex;}

    virtual int id_range () {return lex.size();}
    virtual const char* id2str (int id) {return lex.id2str (id);}
    virtual int str2id (const char *str) {return lex.str2id (str);}
    virtual int pos2id (Position pos) {return txt.pos2id (pos);}
    virtual const char* pos2str (Position pos) 
        {return lex.id2str (txt.pos2id (pos));}
    virtual IDIterator *posat (Position pos) {return new IDIter (txt.at (pos));}
    virtual IDPosIterator *idposat (Position pos)
        {return new IDPosIterator (new IDIter (txt.at (pos)),
                                   new SequenceStream (pos, size()-1, size()));}
    virtual TextIterator *textat (Position pos) 
        {return new TextIter (txt.at (pos), lex);}
    virtual FastStream *id2poss (int id) {return rev.id2poss (id);}
    virtual FastStream *compare2poss (const char *pat, int cmp, bool ignorecase) 
        {return ::compare2poss (rev, lex, pat, cmp, ignorecase);}
    virtual FastStream *regexp2poss (const char *pat, bool ignorecase) {
        if (regex) {
            FastStream *fs = optimize_regex (regex, pat, encoding);
            return ::regexp2poss (rev, lex, pat, locale, encoding, ignorecase, fs);
        }
        return ::regexp2poss (rev, lex, pat, locale, encoding, ignorecase);
    }
    virtual Generator<int> *regexp2ids (const char *pat, bool ignorecase, const char *filter_pat) {
        if (regex) {
            FastStream *fs = optimize_regex (regex, pat, encoding);
            return ::regexp2ids (lex, pat, locale, encoding, ignorecase, filter_pat, fs);
        }
        return ::regexp2ids (lex, pat, locale, encoding, ignorecase, filter_pat);
    }
    virtual NumOfPos freq (int id) {return rev.count (id);}
    virtual NumOfPos docf (int id) {return docff ? (*docff)[id] : -1LL;}
    virtual float arf (int id) {return arff ? (*arff)[id] : -1LL;}
    virtual float aldf (int id) {return aldff ? (*aldff)[id] : -1LL;}
    virtual NumOfPos norm (int id) {return normf ? (*normf)[id] : freq (id);}
    virtual NumOfPos size() {return txt.size();}
};




//-------------------- GenPosAttr instances --------------------

typedef GenPosAttr<map_delta_revidx, giga_delta_text<MapBinFile<uint8_t> >, map_lexicon>
        MD_MGD_PosAttr;
typedef GenPosAttr<map_delta_revidx, map_delta_text, map_lexicon> 
        MD_MD_PosAttr;
typedef GenPosAttr<file_delta_revidx, map_delta_text, map_lexicon> 
        FD_MD_PosAttr;
typedef GenPosAttr<file_delta_revidx, file_delta_text, map_lexicon> 
        FD_FD_PosAttr;
typedef GenPosAttr<file_file_delta_revidx, file_delta_text, map_lexicon> 
        FFD_FD_PosAttr;
typedef GenPosAttr<file_delta_revidx, file_big_delta_text, map_lexicon> 
        FD_FBD_PosAttr;
typedef GenPosAttr<file_delta_revidx, file_giga_delta_text, map_lexicon> 
        FD_FGD_PosAttr;
typedef GenPosAttr<file_file_delta_revidx, file_file_giga_delta_text, 
                   map_lexicon, BinFile<int> > NoMem_PosAttr;
typedef GenPosAttr<map_delta_revidx, int_text, map_lexicon> MD_MI_PosAttr;
typedef GenPosAttr<file_delta_revidx, int_text, map_lexicon> FD_MI_PosAttr;

PosAttr *createPosAttr (string &typecode, const string &path, const string &n,
                        const string &locale, const string &encoding, 
                        NumOfPos text_size)
{
#define TEST_CODE(X) if (typecode == #X) \
    return new X##_PosAttr (path, n, locale, encoding, text_size)

    if (typecode == "default")
        return new MD_MD_PosAttr (path, n, locale, encoding, text_size);
    else if (typecode == "UNIQUE")
        return createUniqPosAttr (path, n, locale, encoding, text_size);
    else TEST_CODE (MD_MGD);
    else TEST_CODE (MD_MD);
    else TEST_CODE (FD_MD);
    else TEST_CODE (FD_FD);
    else TEST_CODE (FFD_FD);
    else TEST_CODE (FD_FBD);
    else TEST_CODE (FD_FGD);
    else TEST_CODE (NoMem);
    else TEST_CODE (MD_MI);
    else TEST_CODE (FD_MI);
    else throw AttrNotFound ("Uknown type: " + typecode + ", " + path);

#undef TEST_CODE
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

