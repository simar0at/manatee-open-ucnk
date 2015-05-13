//  Copyright (c) 1999-2013  Pavel Rychly

#ifndef CORPUS_HH
#define CORPUS_HH

#include <finlib/range.hh>
#include "posattr.hh"
#include "corpconf.hh"
#include "virtcorp.hh"
#include "excep.hh"
#include <vector>

class Structure;

class Corpus {
public:
    typedef enum {Corpus_type, Struct_type} type_t;
    typedef std::vector<std::pair<std::string,PosAttr*> > VSA;
    typedef std::vector<std::pair<std::string,Structure*> > VSS;
    typedef std::vector<std::pair<std::string,Corpus*> > VSC;
protected:
    void init (CorpInfo *ci);
    Corpus (CorpInfo *ci, type_t t=Corpus_type);
    VSA attrs;
    VSS structs;
    VSC aligned;
    PosAttr *defaultattr;
    int maxctx;
    int hardcut;
    VirtualCorpus *virt;
    virtual PosAttr *setup_attr (const std::string &name);
    virtual Structure *setup_struct (const std::string &name);
    PosAttr *get_struct_pos_attr (const std::string &strname,
                                  const std::string &attname);
    friend void *eval_query_thread (void *conc);
    friend class Concordance;
public:
    type_t type;
    CorpInfo *conf;

    Corpus (const std::string &corp_name);
    virtual ~Corpus ();
    virtual RangeStream *filter_query (RangeStream *s) {return s;}
    PosAttr *get_attr (const std::string &name);
    PosAttr *get_default_attr();
    void set_default_attr (const std::string &attname);
    Structure *get_struct (const std::string &name);
    virtual Position size() {
        return get_default_attr()->size();
    }
    virtual Position search_size() {return size();}
    virtual std::string get_info();
    const std::string &get_conf (const std::string &item) {
        return conf->find_opt (item);
    }
    const std::string &get_confpath () {
        return conf->conffile;
    }
    const char *get_conffile () {
        const char *path = conf->conffile.c_str();
        size_t slash = conf->conffile.rfind("/");
        if (slash != std::string::npos)
            path += slash + 1;
        return path;
    }
    int get_hardcut() {return hardcut;}
    int get_maxctx() {return maxctx;}
    Corpus *get_aligned (const std::string &corp_name);
    virtual RangeStream *map_aligned (Corpus *al_corp, RangeStream *src);
    const std::string get_sizes();
    virtual NumOfPos compute_docf(FastStream *poss, RangeStream *struc);
    void freq_dist (RangeStream *r, std::ostream &out, const char *crit,
                    NumOfPos limit);
    void freq_dist (RangeStream *r, const char *crit, NumOfPos limit,
                    std::vector<std::string> &words, std::vector<NumOfPos> &freqs,
                    std::vector<NumOfPos> &norms);
};


class Structure: public Corpus {
public:
    ranges *rng;
    const std::string name;
    std::string endtagstring;
    Structure (CorpInfo *i, const std::string &path, const std::string &n);
    Structure (CorpInfo *i, const std::string &n, VirtualCorpus *vc);
    virtual ~Structure() {delete rng;}
    virtual Position size () {return rng->size();}
//      RangeStream *whole();
//      RangeStream *part (FastStream *filter);
};


#endif

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

