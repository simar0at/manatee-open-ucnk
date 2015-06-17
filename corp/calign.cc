//  Copyright (c) 2007-2012  Pavel Rychly, Milos Jakubicek

#include "corpus.hh"
#include "frsop.hh"
#include "levels.hh"
#include <sstream>
#include <iostream>

using namespace std;


class StructNums: public FastStream {
protected:
    ranges *rng;
    RangeStream *src;
    Position currnum;
    Position blockend;
    NumOfPos finval;
public:
    StructNums (ranges *rngi, RangeStream *srci)
        : rng(rngi), src(srci), currnum(-1), finval(rng->size()) {find(0);}
    virtual ~StructNums() {delete src;}
    virtual Position peek() {return currnum;}
    virtual Position next() {
        Position ret = currnum;
        if (currnum < finval && ++currnum > blockend)
            find (currnum);
        return ret;
    }
    virtual Position find (Position pos) {
        if (currnum == finval || src->end()) {
            currnum = finval;
            blockend = finval;
        } else {
            src->find_beg(rng->beg_at(pos));
            Position s = rng->num_next_pos(src->peek_beg());
            if (currnum < s)
                currnum = s;
            blockend = rng->num_next_pos(src->peek_end() -1);
        }
        return currnum;
    }
    virtual NumOfPos rest_min() {return src->rest_min();}
    virtual NumOfPos rest_max() {return src->rest_max();}
    virtual Position final() {return finval;}
    virtual void add_labels(Labels &lab) {src->add_labels (lab);}
};

Corpus *Corpus::get_aligned (const string &corp_name)
{
    for (unsigned c = 0; c < aligned.size(); c++)
        if (aligned[c].corp_name == corp_name) {
            if (!aligned[c].corp)
                aligned[c].corp = new Corpus (corp_name);
            return aligned[c].corp;
        }
    throw CorpInfoNotFound (corp_name + " not aligned");
}

TokenLevel *Corpus::get_aligned_level (const string &corp_name)
{
    const string levelfile = get_conf ("PATH") + "align." + corp_name;
    for (unsigned c = 0; c < aligned.size(); c++)
        if (aligned[c].corp_name == corp_name) {
            if (!aligned[c].level)
                aligned[c].level = new_TokenLevel (levelfile);
            return aligned[c].level;
        }
    throw CorpInfoNotFound (corp_name + " not aligned");
}

RangeStream *Corpus::map_aligned (Corpus *al_corp, RangeStream *src)
{
    int corp_num = -1;
    for (unsigned i = 0; i < aligned.size(); i++)
        if (aligned[i].corp == al_corp) {
            corp_num = i;
            break;
        }

    if (corp_num == -1)
        throw CorpInfoNotFound (al_corp->get_confpath() + " not aligned");
    src = new AddRSLabel (src, (corp_num + 1) * 100);
    
    Structure *als = al_corp->get_struct(al_corp->get_conf("ALIGNSTRUCT"));
    FastStream *snums = new StructNums(als->rng, src);
    if (! al_corp->get_conf("ALIGNDEF").empty()) 
        snums = tolevelfs (al_corp->get_aligned_level (get_conffile()),
                           snums);
    return get_struct(get_conf("ALIGNSTRUCT"))->rng->part (snums);
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
