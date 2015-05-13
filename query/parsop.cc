//  Copyright (c) 2003-2010  Pavel Rychly

#include "parsop.hh"

// ------------------------- RQFilterPos ----------------------------

RQFilterPos::RQFilterPos (RangeStream *rs, PosAttr *a1, PosAttr *a2, Op o, 
                          int arg1, int arg2)
    : src (rs), attr1 (a1), attr2 (a2), op (o), arg1 (arg1), arg2 (arg2)
{
    active = src->peek_beg() < src->final(); 
    locate();
}


void RQFilterPos::locate()
{
    if (!active) return;
    Labels lab;
    do {
        lab.clear();
        src->add_labels (lab);
        int id1 = attr1->pos2id (lab [arg1]);
        int id2 = attr2->pos2id (lab [arg2]);
        if (op == F_EQ && id1 == id2) break;
        if (op == F_NEQ && id1 != id2) break;
    } while ((active = src->next()));
}

bool RQFilterPos::next()
{
    active = src->next();
    locate();
    return active;
}

Position RQFilterPos::find_beg (Position pos)
{
    active = src->find_beg (pos) < src->final();
    locate();
    return src->peek_beg();
}

Position RQFilterPos::find_end (Position pos)
{
    active = src->find_end (pos) < src->final();
    locate();
    return src->peek_beg();
}

RQFilterPos::~RQFilterPos () {delete src;}
Position RQFilterPos::peek_beg() const {return src->peek_beg();}
Position RQFilterPos::peek_end() const {return src->peek_end();}
void RQFilterPos::add_labels (Labels &lab) const {src->add_labels (lab);}
NumOfPos RQFilterPos::rest_min() const {return src->rest_min();}
NumOfPos RQFilterPos::rest_max() const {return src->rest_max();}
Position RQFilterPos::final() const {return src->final();}
int RQFilterPos::nesting() const {return src->nesting();}
bool RQFilterPos::epsilon() const {return src->epsilon();}

// ------------------------- RQFilterFreq ----------------------------

RQFilterFreq::RQFilterFreq (RangeStream *rs, PosAttr *a1, Op o, int neg, int arg1, NumOfPos freq)
    : src (rs), attr1 (a1), op (o), neg (neg), arg1 (arg1), freq (freq)
{
    active = src->peek_beg() < src->final(); 
    locate();
}


void RQFilterFreq::locate()
{
    if (!active) return;
    Labels lab;
    do {
        lab.clear();
        src->add_labels (lab);
        int id1 = attr1->pos2id (lab [arg1]);
        if (op == F_EQ && ((attr1->freq(id1) == freq) == !neg)) break;
        if (op == F_LEQ && ((attr1->freq(id1) <= freq) == !neg)) break;
        if (op == F_GEQ && ((attr1->freq(id1) >= freq) == !neg)) break;
    } while ((active = src->next()));
}

bool RQFilterFreq::next()
{
    active = src->next();
    locate();
    return active;
}

Position RQFilterFreq::find_beg (Position pos)
{
    active = src->find_beg (pos) < src->final();
    locate();
    return src->peek_beg();
}

Position RQFilterFreq::find_end (Position pos)
{
    active = src->find_end (pos) < src->final();
    locate();
    return src->peek_beg();
}

RQFilterFreq::~RQFilterFreq () {delete src;}
Position RQFilterFreq::peek_beg() const {return src->peek_beg();}
Position RQFilterFreq::peek_end() const {return src->peek_end();}
void RQFilterFreq::add_labels (Labels &lab) const {src->add_labels (lab);}
NumOfPos RQFilterFreq::rest_min() const {return src->rest_min();}
NumOfPos RQFilterFreq::rest_max() const {return src->rest_max();}
Position RQFilterFreq::final() const {return src->final();}
int RQFilterFreq::nesting() const {return src->nesting();}
bool RQFilterFreq::epsilon() const {return src->epsilon();}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
