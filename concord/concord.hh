//  Copyright (c) 1999-2013  Pavel Rychly

#ifndef CONCORD_HH
#define CONCORD_HH

#include "corpus.hh"
#include "config.hh"

#ifdef HAVE_PTHREAD
    #include <pthread.h>
    #define mutex_t pthread_mutex_t
    #define thread_t pthread_t
    #define mutex_init(...) pthread_mutex_init (__VA_ARGS__)
    #define mutex_destroy(...) pthread_mutex_destroy (__VA_ARGS__)
    #define mutex_lock(...) pthread_mutex_lock (__VA_ARGS__)
    #define mutex_trylock(...) pthread_mutex_trylock (__VA_ARGS__)
    #define mutex_unlock(...) pthread_mutex_unlock (__VA_ARGS__)
    #define thread_create(...) pthread_create (__VA_ARGS__)
    #define thread_join(...) pthread_join (__VA_ARGS__)
    #define thread_cleanup_push(...) pthread_cleanup_push (__VA_ARGS__)
    #define thread_cleanup_pop(...) pthread_cleanup_pop (__VA_ARGS__)
    #define thread_setcancelstate(...) pthread_setcancelstate (__VA_ARGS__)
    #define thread_testcancel(...) pthread_testcancel(__VA_ARGS__)
    #define thread_cancel(...) pthread_cancel(__VA_ARGS__)
#else
    #define mutex_t int
    #define thread_t int
    #define mutex_init(...)
    #define mutex_destroy(...)
    #define mutex_lock(...)
    #define mutex_trylock(...)
    #define mutex_unlock(...)
    #define thread_create(id, attr, func, arg) func(arg)
    #define thread_join(...)
    #define thread_cleanup_push(...)
    #define thread_cleanup_pop(...)
    #define thread_setcancelstate(...)
    #define thread_testcancel(...)
    #define thread_cancel(...)
#endif

#include <iostream>
#include <stdint.h>

#if defined HAVE_EXT_HASH_MAP
#include <ext/hash_map>
#    if defined HAVE_GNU_CXX_NS
         namespace std {using namespace __gnu_cxx;}
#    endif
#else
#include <hash_map>
#endif

class ConcNotFound: public std::exception {
    const std::string _what;
public:
    const std::string name;
    ConcNotFound (const std::string &name)
        :_what ("Concordance `" + name + "' not defined"), name (name) {}
    virtual const char* what () const throw () {return _what.c_str();}
    virtual ~ConcNotFound() throw () {}
};

typedef int32_t ConcIndex; // must be signed

struct ConcItem {
    Position beg;
    Position end;
    ConcItem () {}
    ConcItem (Position b, Position e): beg(b), end(e) {}
};

struct collocitem {
    int8_t beg, end;
};

class ConcStream; // forward declare

class Concordance {
public:
    //-------------------- Context --------------------
    class context {
    public:
        int chars;
        context (int ch=0): chars (ch) {}
        virtual ~context () {};
        virtual Position get (RangeStream *r) = 0;
    };
    class criteria;
protected:
    //-------------------- Collocations --------------------
    std::vector<collocitem*> colls;
    std::vector<ConcIndex> coll_count;
    static const int cnotdef = -128;

    //-------------------- private members --------------------
    ConcItem *rng;
    int nestval;
    ConcIndex allocated, used;
    std::vector<ConcIndex> *view;
    typedef std::vector<short int> linegroup_t;
    linegroup_t *linegroup;
    mutex_t *rng_mutex;
    thread_t *thread_id;
    RangeStream *query;
    bool is_finished;
    ConcIndex sample_size;
    NumOfPos full_size;
    int label;
    bool added_align; // true for parallel corpus added by add_aligned()
    //------------------- CorpData ------------------------
    struct CorpData {
        std::vector<collocitem*> colls;
        std::vector<ConcIndex> coll_count;
        ConcItem *rng;
        Corpus *corp;
        int label;
        bool added_align; // true for parallel corpus added by add_aligned()
    };
    std::vector<CorpData*> aligned;
    
    struct set_collocation_data;
    friend void *evaluate_query (Concordance *conc);
    friend void *evaluate_colloc (set_collocation_data *data);
    friend void *eval_query_thread (void *conc);
    friend void free_rngmutex (void *conc);
    friend class ConcStream;
    void lock() {
        if (rng_mutex)
            mutex_lock(rng_mutex);
    }
    void unlock() {
        if (rng_mutex)
            mutex_unlock(rng_mutex);
    }
    void ensure_view();
    template <class ConcData>
    void delete_lines (ConcData *data, ConcIndex newsize, int collnum,
                       bool positive, std::vector<ConcIndex> *view,
                       std::vector<ConcIndex> *revview);
    //-------------------- public members --------------------
public:
    static const int lngroup_labidx;
    Corpus * corp;
    NumOfPos corp_size;

    Concordance (Corpus *corp, RangeStream *query, int inccolln=0,
                 ConcIndex sample_size=0, NumOfPos full_size=0);
    Concordance (Concordance &x);
    Concordance (Corpus *corp, const char *filename);
    ~Concordance ();
    void save (const char *filename, bool save_linegroup=false,
               bool partial=false, bool append=false);
    void sync ();
    ConcIndex size() const {return used;}
    int nesting() const {return nestval;}
    int numofcolls() const {return colls.size();}
    ConcIndex viewsize() const {return view ? view->size() : size();}
    NumOfPos fullsize() const {return full_size > 0 ? full_size : size();}
    bool finished() const {return is_finished;}

    void set_collocation (int collnum, const std::string &cquery, 
                          const char *lctx, const char *rctx, int rank,
                          bool exclude_kwic = false);

    Position beg_at (ConcIndex idx) {
        lock();
        Position ret = rng[idx].beg;
        unlock();
        return ret;
    }
    Position end_at (ConcIndex idx) {
        lock();
        Position ret = rng[idx].end;
        unlock();
        return ret;
    }
    Position coll_beg_at (int coll, ConcIndex idx) {
        //XXX zamykani
        if (coll <= 0 || unsigned (coll) > colls.size())
            return beg_at (idx);
        if (! colls[coll-1] || beg_at (idx) == -1)
            return -1;
        int d = colls [coll-1][idx].beg;
        return d == cnotdef ? -1 : beg_at (idx) + d;
    }
    Position coll_end_at (int coll, ConcIndex idx) {
        //XXX zamykani
        if (coll <= 0 || unsigned (coll) > colls.size())
            return end_at (idx);
        if (! colls[coll-1] || beg_at (idx) == -1)
            return -1;
        Position d = colls [coll-1][idx].end;
        return d == cnotdef ? -1 : beg_at (idx) + d;
    }
    FastStream *begs_FS();
    RangeStream *RS (bool useview = false, ConcIndex beg = 0, ConcIndex end=0);
    void get_aligned (std::vector<std::string> &corpnames);

    //-------------------- linegroups --------------------
    void set_linegroup (ConcIndex linenum, int group);
    void set_linegroup_globally (int group);
    int set_linegroup_at_pos (Position pos, int group);
    void set_linegroup_from_conc (Concordance *master);
    int get_linegroup (ConcIndex lineidx) {
        return (linegroup && lineidx >= 0 && lineidx < size()) ? 
            (*linegroup)[lineidx] : 0;
    }
    void get_linegroup_stat (std::map<short int,ConcIndex> &lgs);
    int get_new_linegroup_id();
    void delete_linegroups (const char *grps, bool invert=false);


    //-------------------- KWICLines --------------------
    friend class KWICLines;
    void tcl_get (std::ostream &out, ConcIndex beg, ConcIndex end, 
                  const char *left, const char *right,
                  const char *ctxa, const char *kwica,
                  const char *struca, const char *refa);
    void tcl_get_reflist (std::ostream &out, ConcIndex idx, const char *reflist);

    //-------------------- statistics --------------------
    int distribution (std::vector<int> &vals, std::vector<int> &beginnings,
                      int yrange);
    NumOfPos redfreq ();
    double compute_ARF();
    void sort (const char *crit, bool uniq = 0);
    void count_sort (const char *leftctx, const char *rightctx, 
                     const std::string &attr, bool words_only);
    void relfreq_sort (const char *leftctx, const char *rightctx, 
                       const std::string &attr);
    void linegroup_sort (std::map<short int,std::string> &ordertab);
    /*
     * define view order for the (first part of) the concordance
     *     sorted is a list of line numbers
     */
    void set_sorted_view (const std::vector<ConcIndex> &sorted);
    void find_coll (std::ostream &out, const std::string &attr, 
                    const std::string &cmpr, NumOfPos minfreq, NumOfPos minbgr,
                    int fromw, int tow, int maxlines, 
                    const char *stats="mtrf");
    void sort_idx (const char *crit, std::vector<std::string> &chars,
                   std::vector<int> &idxs, bool firstCharOnly = true);
    void poss_of_selected_lines (std::ostream &out, const char *rngs);
    void delete_lines (const char *rngs);
    void reduce_lines (const char *crit);
    void delete_view_lines (const char *rngs);
    void reduce_view_lines (const char *crit);
    void delete_pnfilter (int collnum, bool positive);
    void swap_kwic_coll (int collnum);
    void extend_kwic_coll (int collnum);
    void shuffle();
    void make_grouping();
    void switch_aligned (const char *corpname);
    void add_aligned (const char *corpname);
};

#endif

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
