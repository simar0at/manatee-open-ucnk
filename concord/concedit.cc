//  Copyright (c) 1999-2013  Pavel Rychly

#include "concord.hh"
#include "levels.hh"
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <cstring>

using namespace std;

struct ConcData {
    ConcData (vector<collocitem*> * c, vector<ConcIndex> * cc, ConcItem **r):
        colls(c), coll_count(cc), rng(r) {}
    vector<collocitem*> * colls;
    vector<ConcIndex> * coll_count;
    ConcItem ** rng;
};

void Concordance::reduce_lines (const char *crit)
{
    sync();
    NumOfPos newsize;
    char ch;
    istringstream in (crit);
    in >> newsize;
    double scale = 1.0;
    while ((in >> ch) && ch == '%')
        scale /= 100;
    if (scale != 1.0)
        newsize = int (newsize * scale * viewsize());
    if (newsize >= viewsize())
        return;
    
    if (view) {
        delete view;
        view = NULL;
    }

    vector<ConcData> concdata;
    for (int i = 0; i < aligned.size(); i++)
        concdata.push_back(ConcData(&aligned[i]->colls, &aligned[i]->coll_count,
                                    &aligned[i]->rng));
    concdata.push_back(ConcData(&colls, &coll_count, &rng));

    for (vector<ConcData>::iterator data = concdata.begin();
         data != concdata.end(); ++data) {
        srand (1);
        double lines = newsize;
        ConcItem *newrng = (ConcItem *) malloc (newsize * sizeof (ConcItem));
        vector<collocitem*> newcolls (data->colls->size());
        vector<ConcIndex> newcoll_count (data->colls->size());
        for (unsigned int i = 0; i < data->colls->size(); i++)
            if (data->colls->at(i))
                newcolls[i] = (collocitem*) malloc(newsize*sizeof(collocitem));

        float process = size();
        Position oldl, newl;
        for (oldl=newl=0; oldl < size() && newl < newsize; oldl++, process--) {
            if (process * rand() / (RAND_MAX + 1.0) <= lines) {
                newrng [newl] = (*data->rng) [oldl];
                for (unsigned int i = 0; i < data->colls->size(); i++) {
                    if (data->colls->at(i)) {
                        newcolls[i][newl] = data->colls->at(i)[oldl];
                        if (newcolls[i][newl].beg != cnotdef)
                            newcoll_count[i] ++;
                    }
                }
                lines--;
                newl++;
            }
        }
        delete *data->rng;
        *data->rng = newrng;
        for (unsigned int i = 0; i < data->colls->size(); i++)
            free (data->colls->at(i));
        *data->colls = newcolls;
        *data->coll_count = newcoll_count;
    }

    used = newsize;
    allocated = newsize;
}

template <class ConcData>
void Concordance::delete_lines (ConcData *data, ConcIndex newsize, int collnum,
                                bool positive, vector<ConcIndex> *view,
                                vector<ConcIndex> *revview)
{
    ConcItem *newrng = (ConcItem *) malloc (newsize * sizeof (ConcItem));
    vector<collocitem*> newcolls (data->colls.size());
    vector<ConcIndex> newcoll_count (data->colls.size());
    for (unsigned int i = 0; i < data->colls.size(); i++)
        if (data->colls[i])
            newcolls[i] = (collocitem*) malloc (newsize * sizeof (collocitem));
    Position oldl, newl;
    for (oldl = newl = 0; oldl < size() && newl < newsize; oldl++) {
        if (data->rng[oldl].beg != -1 &&
            ((coll_beg_at (collnum, oldl) == -1) ^ positive)) {
            newrng [newl] = data->rng [oldl];
            for (unsigned int i = 0; i < data->colls.size(); i++) {
                if (data->colls[i]) {
                    newcolls[i][newl] = data->colls[i][oldl];
                    if (newcolls[i][newl].beg != cnotdef)
                        newcoll_count[i] ++;
                }
            }
            if (revview)
                (*view)[(*revview)[oldl]] = newl;
            newl++;
        }
    }

    delete data->rng;
    data->rng = newrng;
    for (unsigned int i = 0; i < data->colls.size(); i++)
        free (data->colls[i]);
    data->colls = newcolls;
    data->coll_count = newcoll_count;
}

void Concordance::delete_pnfilter (int collnum, bool positive)
{
    sync();
    ConcIndex newsize = coll_count [collnum -1];
    if (! positive)
        newsize = viewsize() - newsize;
    if (newsize == viewsize())
        return;

    vector<ConcIndex> *revview = NULL;
    if (view) {
        ConcIndex vs = view->size();
        revview = new vector<ConcIndex>(allocated, -1);
        for (ConcIndex i=0; i < vs; i++)
            (*revview)[(*view)[i]] = i;
        delete view;
        view = new vector<ConcIndex>(vs, -1);
    }

    for (int c = 0; c < aligned.size(); c++)
        delete_lines (aligned[c], newsize, collnum, positive, NULL, NULL);
    delete_lines (this, newsize, collnum, positive, view, revview);

    used = newsize;
    allocated = newsize;
    if (revview) {
        delete revview;
        vector<ConcIndex>::iterator vi = find (view->begin(), view->end(), -1);
        for (vector<ConcIndex>::iterator si = vi; si != view->end(); si++)
            if (*si != -1)
                *(vi++) = *si;
        if (vi != view->end())
            view->erase (vi, view->end());
    }
}

void Concordance::swap_kwic_coll (int collnum)
{
    sync();
    if (collnum < 1 || unsigned (collnum) > colls.size() 
        || coll_count [collnum -1] == 0)
        return;
    ConcItem *ri = rng;
    collocitem *ci = colls [collnum -1];
    for (ConcItem *last = rng + used; ri < last; ri++, ci++) {
        if (ci->beg == cnotdef) continue;
        int oldend = ri->end;
        ri->end = ri->beg + ci->end;
        ri->beg += ci->beg;
        ci->beg = - ci->beg;
        ci->end = oldend - ri->beg;
    }
}

/*
 * Extends kwic to the given collocation and removes the collocation
 */
void Concordance::extend_kwic_coll (int collnum)
{
    sync();
    if (collnum < 1 || unsigned (collnum) > colls.size()
        || coll_count [collnum -1] == 0)
        return;
    ConcItem *ri = rng;
    collocitem *ci = colls [collnum -1];
    for (ConcItem *last = rng + used; ri < last; ri++, ci++) {
        if (ci->beg == cnotdef) continue;
        if (ci->beg > 0)
            ri->end = ri->beg + ci->end;
        if (ci->beg < 0)
            ri->beg += ci->beg;
        if (ri->beg + ci->end > ri->end)
            ri->end = ri->beg + ci->end;
    }
    free (colls [collnum - 1]);
    colls[collnum - 1] = NULL;
    coll_count [collnum - 1] = 0;
}

/**
 * Generates a random number from 0 to min(i, RAND_MAX).
 * We assume that RAND_MAX is ~ max. available integer
 * (see you stdlib.h for actual value).
 */
int custom_rand(int i) 
{ 
    srand(1298131);
    return rand() % i;
}

/**
 * Shuffles the concordance in a replicable way
 */
void Concordance::shuffle()
{
    sync();
    if (!view) {
        view = new vector<ConcIndex>(size());
        for (ConcIndex i=0; i < size(); i++)
            (*view)[i] = i;
    }
    random_shuffle(view->begin(), view->end(), custom_rand);
}

void Concordance::switch_aligned (const char *corpname)
{
    sync();
    if (!corpname)
        return;
    CorpData *data = NULL;
    for (int i = 0; i < aligned.size(); i++) {
        if (!strcmp(aligned[i]->corp->get_conffile(), corpname)) {
            data = aligned[i];
            break;
        }
    }
    if (!data)
        return;
    CorpData curr_data;
    curr_data.colls = colls;
    curr_data.coll_count = coll_count;
    curr_data.rng = rng;
    curr_data.corp = corp;
    curr_data.label = label;
    curr_data.added_align = added_align;

    colls = data->colls;
    coll_count = data->coll_count;
    rng = data->rng;
    corp = data->corp;
    corp_size = data->corp->size();
    label = data->label;
    added_align = data->added_align;

    data->colls = curr_data.colls;
    data->coll_count = curr_data.coll_count;
    data->rng = curr_data.rng;
    data->corp = curr_data.corp;
    data->label = curr_data.label;
    data->added_align = curr_data.added_align;
}

void Concordance::add_aligned (const char *corpname)
{
    sync();
    if (!corpname || !strcmp(corpname, corp->get_conffile()))
        return;
    for (int i = 0; i < aligned.size(); i++)
        if (!strcmp(aligned[i]->corp->get_conffile(), corpname))
            return; // do not add if present
    CorpData *cdata = new Concordance::CorpData();
    cdata->added_align = true;
    cdata->corp = corp->get_aligned (corpname);
    if (!(cdata->rng = (ConcItem*) malloc (used * sizeof (ConcItem))))
        throw bad_alloc();
    aligned.push_back (cdata);

    Structure *align = corp->get_struct(corp->get_conf("ALIGNSTRUCT"));
    Structure *add_align = cdata->corp->get_struct
        (cdata->corp->get_conf("ALIGNSTRUCT"));
    MLTStream *map = NULL;
    if (! corp->get_conf("ALIGNDEF").empty()) {
        map = full_level (corp->get_conf("PATH") + "align." 
                          + cdata->corp->get_conffile());
    }
    NumOfPos align_num, align_end;
    bool empty; 
    for (int i = 0; i < used; i++) {
        // XXX finding first token of the kwic only
        align_num = align->rng->num_next_pos (rng[i].beg);
        empty = align->rng->beg_at(align_num) > rng[i].beg;
        if (map) {
            map->find_org(align_num);
            if (map->end()) {
                cdata->rng[i].beg = cdata->rng[i].end = cdata->corp->size();
                continue;
            }
            switch (map->change_type()) {
            case MLTStream::KEEP:
                align_num = align_num - map->orgpos() + map->newpos();
                align_end = align_num;
                break;
            case MLTStream::DELETE:
                empty = true;
                /* realy no break */
            default:
                align_num = map->newpos();
                align_end = align_num + map->change_newsize() -1;
            }
        } else {
            align_end = align_num;
        }

        cdata->rng[i].beg = add_align->rng->beg_at (align_num);
        if (empty)
            cdata->rng[i].end = cdata->rng[i].beg;
        else
            cdata->rng[i].end = add_align->rng->end_at (align_end);
    }
    delete map;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
