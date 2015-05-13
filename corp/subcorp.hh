//  Copyright (c) 2002-2011  Pavel Rychly

#ifndef SUBCORP_HH
#define SUBCORP_HH

#include "corpus.hh"
#include <set>

class SubCorpus : public Corpus {
private:
    NumOfPos search_size_cached;
protected:
    ranges *subcorp;
public:
    SubCorpus (const Corpus *corp, const std::string &sub);
    virtual ~SubCorpus ();
    virtual RangeStream *filter_query (RangeStream *s);
    virtual NumOfPos search_size();
    virtual std::string get_info();
    virtual NumOfPos compute_docf(FastStream *poss, RangeStream *struc);
};

void save_subcorpus (const char *subcpath, FastStream *src, Corpus *corp, 
                     ranges *struc);
SubCorpus *create_subcorpus (const char *subcdir, const std::string &corpname,
                             const std::string &subcname, 
                             FastStream *src, Corpus *corp, ranges *struc);
void find_subcorpora (const char *subcdir, std::map<std::string,
                      std::pair<std::string,std::string> > &scs);

#endif

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
