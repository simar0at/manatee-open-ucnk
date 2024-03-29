//  Copyright (c) 1999-2013  Pavel Rychly

#include "concord.hh"
#include "cqpeval.hh"
#include "conccrit.hh"
#include "frsop.hh"
#include <cstdlib>
#include <cstring>

using namespace std;

const int Concordance::lngroup_labidx = 1000000; // 1M;

#define REALLOC(type, pointer, size) \
type *p = (type *) realloc (pointer, size * sizeof (type)); \
if (p) \
    pointer = p; \
else { \
    /* out of memory */ \
    free_rngmutex(conc); \
    conc->is_finished = false; \
    return NULL; /* XXX pthread_setcancelstate */ \
}

void free_rngmutex(void *concordance)
{
    Concordance *conc = static_cast <Concordance*> (concordance);
    mutex_trylock(conc->rng_mutex);
    conc->unlock();
    mutex_destroy(conc->rng_mutex);
    conc->rng_mutex = NULL;
    delete conc->query;
    conc->query = NULL;
    conc->is_finished = true;
}

void *evaluate_colloc (Concordance::set_collocation_data *data);

/* Fills result with sample-random values from the interval <add, add + max)
 * and heapifies result */
void generate_random (vector <ConcIndex> &result, ConcIndex add,
                      ConcIndex sample, ConcIndex max)
{
    hash_map <ConcIndex,int> rand_map;
    rand_map.resize (sample);
    for (ConcIndex i = 0; i < sample; i++) {
        ConcIndex randci = max * (rand() / float(RAND_MAX));
        bool inserted = false;
        int oldsize = rand_map.size();
        rand_map[randci] = 0;
        if (rand_map.size() > oldsize)
            inserted = true;
        int off = 1, flip = 1;
        bool do_flip = true;
        while (!inserted) {// already present
            ConcIndex near_randci = randci + off * flip;
            if (near_randci < 0 || near_randci >= max) {
                flip = -flip;
                do_flip = false;
                near_randci = randci + off * flip;
            }
            if (do_flip)
                flip = -flip;
            oldsize = rand_map.size();
            rand_map[near_randci] = 0;
            if (rand_map.size() > oldsize)
                inserted = true;
            off++;
        }
    }
    result.reserve (sample);
    for (hash_map <ConcIndex,int>::const_iterator it = rand_map.begin();
         it != rand_map.end(); ++it)
        result.push_back (add + it->first);
    make_heap (result.begin(), result.end(), greater <ConcIndex>());
}

void *eval_query_thread (void *concordance) {
    int oldstate;
    thread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);
    Concordance *conc = static_cast <Concordance*> (concordance);
    RangeStream *src = conc->query;
    conc->nestval = src->nesting();
    Position last = src->final();
    Position p;
    int lastcoll = conc->colls.size(), seg_num = 0;
    bool compute_full_size = false;
    if (conc->full_size == -1)
        compute_full_size = true;
    bool sample_online = false;
    ConcIndex curr_seek = 0, rand_seg = 100000;
    if (conc->sample_size && conc->sample_size < conc->full_size)
        sample_online = true;
    vector <ConcIndex> rands;
    NumOfPos full_seg = rand_seg / float(conc->sample_size) * conc->full_size;
    while ((p = src->peek_beg()) < last) {
        // MACRO BEGIN, see man pthread_cleanup_push, section NOTES
        thread_cleanup_push (free_rngmutex, conc);
        thread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldstate);
        thread_testcancel();
        thread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);
        if (sample_online) {
            if (rands.empty())
                generate_random (rands, full_seg * seg_num++,
                                 min (rand_seg, conc->sample_size - conc->used),
                                 min (full_seg, conc->full_size - curr_seek));
            while ((p = src->peek_beg()) < last && curr_seek++ < rands.front())
                src->next();
            pop_heap (rands.begin(), rands.end(), greater <ConcIndex>());
            rands.pop_back();
        }
        Labels lab;
        src->add_labels(lab);
        for (Labels::iterator labi = lab.upper_bound(99); labi != lab.end();
             labi++) {
            int al_num = labi->first / 100;
            Concordance::CorpData *cdata = NULL;
            for (int c = 0; c < conc->aligned.size(); c++) {
                if (conc->aligned [c]->label == al_num) {
                    cdata = conc->aligned [c];
                    break;
                }
            }
            if (!cdata) {
                cdata = new Concordance::CorpData();
                cdata->corp = conc->corp->aligned [al_num - 1].second;
                cdata->label = al_num;
                cdata->added_align = false;
                conc->aligned.push_back (cdata);
                REALLOC (ConcItem, cdata->rng, conc->allocated);
                memset(cdata->rng, 0, conc->allocated * sizeof(ConcItem));
            }
            if (cdata->colls.size() < labi->first % 100) {
                cdata->colls.push_back (NULL);
                cdata->coll_count.push_back (0);
            }
        }
        if (conc->used == conc->allocated) {
            size_t newsize = conc->used + 512;
            conc->lock();
            REALLOC (ConcItem, conc->rng, newsize);
            conc->allocated = newsize;
            for (int i = 0; i < lastcoll; i++) {
                REALLOC (collocitem, conc->colls[i], newsize);
            }
            for (int c = 0; c < conc->aligned.size(); c++) {
                Concordance::CorpData *cdata = conc->aligned [c];
                REALLOC (ConcItem, cdata->rng, newsize);
                for (int i = 0; i < cdata->colls.size(); i++) {
                    REALLOC (collocitem, cdata->colls[i], newsize);
                }
            }
            conc->unlock();
        }
        conc->rng[conc->used].beg = p;
        conc->rng[conc->used].end = src->peek_end();
        if (lastcoll > 0) {
            for (int i = 0; i < lastcoll && i < 100; i++) {
                collocitem &ci = conc->colls[i][conc->used];
                Labels::iterator labi = lab.find(i+1);
                if (labi == lab.end())
                    ci.beg = ci.end = Concordance::cnotdef;
                else {
                    ci.beg = labi->second - p;
                    ci.end = ci.beg + 1;
                    conc->coll_count[i]++;
                }
            }
        }
        for (int c = 0; c < conc->aligned.size(); c++) {
            Concordance::CorpData *cdata = conc->aligned [c];
            int lab_num = 100 * cdata->label;
            ConcItem &ci = cdata->rng [conc->used];
            Labels::iterator lab_beg = lab.find(lab_num);
            Labels::iterator lab_end = lab.find(-lab_num);
            if (lab_beg == lab.end()) {
                ci.beg = ci.end = Concordance::cnotdef;
            } else {
                ci.beg = lab_beg->second;
                if (lab_end == lab.end())
                    ci.end = ci.beg + 1;
                else
                    ci.end = lab_end->second;
            }
            for (int i = 0; i < cdata->colls.size(); i++) {
                collocitem &ci = cdata->colls[i][conc->used];
                int coll_lab_num = lab_num + i;
                Labels::iterator lab_beg = lab.find(coll_lab_num);
                Labels::iterator lab_end = lab.find(-coll_lab_num);
                if (lab_beg == lab.end())
                    ci.beg = ci.end = Concordance::cnotdef;
                else {
                    ci.beg = lab_beg->second - p;
                    if (lab_end == lab.end())
                        ci.end = ci.beg + 1;
                    else
                        ci.end = lab_end->second;
                    cdata->coll_count[i]++;
                }
            }
        }
        conc->used++;
        src->next();
        thread_cleanup_pop (0); // MACRO END
        if (conc->sample_size == conc->used) {
            if (compute_full_size) {
                conc->full_size = conc->used;
                while ((p = src->peek_beg()) < last) {
                    conc->full_size++;
                    src->next();
                }
            }
            break;
        }
    }
    thread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldstate);
    free_rngmutex(conc);
    return NULL;
}

Concordance::Concordance (Corpus *corp, RangeStream *query, int inccolln,
                          ConcIndex sample_size, NumOfPos full_size)
    : rng (NULL), allocated (0), used (0), 
      view (NULL), linegroup (NULL), query (query), corp (corp), 
      corp_size (corp->size()), is_finished (false), sample_size (sample_size),
      full_size (full_size), label (0), added_align (false)
{
    if (!query)
        return;
    if (!sample_size)
        sample_size = corp->get_hardcut();
    for (int i=0; i < inccolln; i++) {
        colls.push_back(NULL);
        coll_count.push_back(0);
    }
    thread_id = new thread_t;
    rng_mutex = new mutex_t;
    mutex_init(rng_mutex, NULL);
    thread_create(thread_id, NULL, eval_query_thread, this);
}

Concordance::Concordance (Concordance &x)
    : view (NULL), linegroup (NULL), rng_mutex (NULL), thread_id (NULL),
      query (NULL), corp (x.corp), corp_size (x.corp_size),
      is_finished (x.finished()), sample_size (x.sample_size),
      full_size (x.full_size), label (x.label), added_align (x.added_align)
{
    x.sync();
    nestval = x.nestval;
    allocated = x.allocated;
    used = x.used;

    rng = (ConcItem *) malloc (x.size() * sizeof (ConcItem));
    if (!rng)
        throw bad_alloc();
    memcpy (rng, x.rng, x.size() * sizeof (ConcItem));
    if (x.view)
        view = new vector<ConcIndex> (x.view->begin(), x.view->end());
    if (x.linegroup)
        linegroup = new linegroup_t (x.linegroup->begin(),
                                     x.linegroup->end());
    if (!x.colls.empty()) {
        for (unsigned int i=0; i < x.colls.size(); i++) {
            coll_count.push_back (x.coll_count[i]);
            colls.push_back ((collocitem *)
                             malloc (x.size() * sizeof (collocitem)));
            if (!colls[i])
                throw bad_alloc();
            memcpy (colls[i], x.colls[i], x.size() * sizeof (collocitem));
        }
    }
}

Concordance::~Concordance ()
{
    if (thread_id) {
        thread_cancel (*thread_id);
        thread_join (*thread_id, NULL);
        delete thread_id;
    }
    if (rng_mutex) {
        mutex_destroy(rng_mutex);
        delete rng_mutex;
    }
    if (view)
        delete view;
    if (linegroup)
        delete linegroup;
    for (unsigned int i=0; i < colls.size(); i++)
        free (colls[i]);
    free (rng);
}

void Concordance::sync()
{
    if (thread_id) {
        thread_join (*thread_id, NULL);
        delete thread_id;
        thread_id = NULL;
    }
}

void Concordance::ensure_view ()
{
    if (!view) {
        view = new vector<ConcIndex>(size());
        for (ConcIndex i=0; i < size(); i++)
            (*view)[i] = i;
    }
}


//XXX void Concordance::ensure_compact ()
//        vyhazet -1

struct Concordance::set_collocation_data {
    string query;
    int collnum;
    Concordance *conc;
    context *leftctx, *rightctx;
    int rank;
    bool exclude_kwic;
    set_collocation_data (const string &q, int n, Concordance *c, int rn,
                          context *l, context *r, bool ek)
        : query (q), collnum (n), conc(c), leftctx (l), rightctx (r),
          rank (rn), exclude_kwic (ek)
    {}
};

void *evaluate_colloc (Concordance::set_collocation_data *data)
{
    Concordance *conc = data->conc;
    RangeStream *src = eval_cqpquery (data->query.c_str(), conc->corp);
    src = new RSFindBack (src);
    collocitem *cis = (collocitem*) malloc (sizeof (collocitem) * conc->size());
    //XXX conc->nestval = src->nesting();
    int coll_count = 0;
    RangeStream *concrs = conc->RS();
    for (ConcIndex idx=0; idx < conc->size(); idx++) {
        if (conc->beg_at (idx) == -1) {
            cis[idx].beg = cis[idx].end = Concordance::cnotdef;
            concrs->next();
            continue;
        }
        Position ctxbeg = data->leftctx->get (concrs);
        Position ctxend = data->rightctx->get (concrs) +1;
        concrs->next();
        src->find_beg (ctxbeg);
        int r = data->rank;
        if (r == 0) { // exact match
            Position b,e;
            while ((b = src->peek_beg()) == ctxbeg &&
                   (e = src->peek_end()) < ctxend)
                src->next();
            if (b == ctxbeg && e == ctxend && !src->end()) {
                cis[idx].beg = b - conc->beg_at (idx);
                cis[idx].end = e - conc->beg_at (idx);
                coll_count++;
            } else {
                cis[idx].beg = cis[idx].end = Concordance::cnotdef;
            }
        } else if (r > 0) {
            // r-ty
            Position b,e;
            while ((b = src->peek_beg()) >= ctxbeg && 
                   (e = src->peek_end()) <= ctxend && r) {
                if (!data->exclude_kwic
                    || (e <= conc->beg_at (idx) || b >= conc->end_at (idx)))
                    r--;
                if (r)
                    src->next();
            }
            e = src->peek_end();
            if (b >= ctxbeg && e <= ctxend && !r && !src->end()) {
                cis[idx].beg = b - conc->beg_at (idx);
                cis[idx].end = e - conc->beg_at (idx);
                coll_count++;
            } else {
                cis[idx].beg = cis[idx].end = Concordance::cnotdef;
            }
        } else {
            // posledni
            r = -r;
            vector <pair<Position, Position> > buff;
            buff.reserve (r);
            Position b,e;
            while ((b = src->peek_beg()) >= ctxbeg && 
                   (e = src->peek_end()) <= ctxend && int (buff.size()) < r) {
                if (!data->exclude_kwic
                    || (e <= conc->beg_at (idx) || b >= conc->end_at (idx)))
                    buff.push_back (make_pair(b,e));
                src->next();
            }
            while ((b = src->peek_beg()) >= ctxbeg && 
                   (e = src->peek_end()) <= ctxend) {
                if (!data->exclude_kwic
                    || (e <= conc->beg_at (idx) || b >= conc->end_at (idx))) {
                    buff.erase (buff.begin());
                    buff.push_back (make_pair(b,e));
                }
                src->next();
            }
            
            if (int (buff.size()) == r && !src->end()) {
                cis[idx].beg = buff[0].first - conc->beg_at (idx);
                cis[idx].end = buff[0].second - conc->beg_at (idx);
                coll_count++;
            } else {
                cis[idx].beg = cis[idx].end = Concordance::cnotdef;
            }
        }
    }

    conc->colls [data->collnum] = cis;
    conc->coll_count [data->collnum] = coll_count;
    delete src;
    delete concrs;
    delete data;
    return NULL;
}

void Concordance::set_collocation (int collnum, const string &cquery, 
                                   const char *lctx, const char *rctx, 
                                   int rank, bool exclude_kwic)
{
    sync();
    while (colls.size() < unsigned (collnum)) {
        colls.push_back (NULL);
        coll_count.push_back (0);
    }
    if (colls [--collnum]) {
        free (colls [collnum]);
        colls [collnum] = NULL;
        coll_count [collnum] = 0;
    }
    //qeval = new pthread_t;
    //int err = pthread_create (qeval, NULL, evaluate_colloc, 
    evaluate_colloc (
         new set_collocation_data (cquery, collnum, this, rank,
                                   prepare_context (this->corp, lctx, true),
                                   prepare_context (this->corp, rctx, false),
                                   exclude_kwic));
}

void Concordance::get_aligned (std::vector<std::string> &corpnames)
{
    for (int i = 0; i < aligned.size(); i++)
        if (!aligned [i]->added_align)
            corpnames.push_back (aligned [i]->corp->get_conffile());
    if (!added_align)
        corpnames.push_back (corp->get_conffile());
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
