// Copyright (c) 1999-2012  Pavel Rychly

#ifndef POSATTR_HH
#define POSATTR_HH

#include "excep.hh"
#include <finlib/fstream.hh>
#include <finlib/generator.hh>
#include <exception>

class CorpInfo;

class IDIterator
{
public:
    virtual int next() =0;
    virtual ~IDIterator() {}
};

class TextIterator
{
public:
    virtual const char *next() =0;
    virtual ~TextIterator() {}
};

class PosAttr 
{
public:
    const std::string attr_path;
    const std::string name;
    const char *locale;
    const char *encoding;
    PosAttr (const std::string &path, const std::string &n, 
             const std::string &loc="", const std::string &enc="");
    virtual ~PosAttr ();
    virtual int id_range () =0;
    virtual const char* id2str (int id) =0;
    virtual int str2id (const char *str) =0;
    virtual int pos2id (Position pos) =0;
    virtual const char* pos2str (Position pos) =0;
    virtual IDIterator *posat (Position pos) =0;
    virtual TextIterator *textat (Position pos) =0;
    virtual FastStream *id2poss (int id) =0;
    virtual FastStream *regexp2poss (const char *pat, bool ignorecase) =0;
    virtual FastStream *compare2poss (const char *pat, int cmp, bool ignorecase) =0;
    virtual Generator<int> *regexp2ids (const char *pat, bool ignorecase, const char *filter_pat = NULL) =0;
    virtual NumOfPos freq (int id) =0;
    virtual NumOfPos norm (int id) =0;
    virtual NumOfPos size() =0;
};


const char *locale2c_str (const std::string &locale);


PosAttr *createPosAttr (std::string &typecode, const std::string &path, 
                        const std::string &n, const std::string &locale, 
                        const std::string &encoding, NumOfPos text_size=0);


PosAttr *findPosAttr (const std::string &corpus_name, 
                      const std::string &attr_name);
PosAttr *findPosAttr (CorpInfo *ci, const std::string &attr_name);

#endif

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

