//  Copyright (c) 1999-2013  Pavel Rychly

#include "concord.hh"

class ConcStream : public RangeStream {
    Concordance *conc;
    bool useview;
    ConcIndex curr, endidx;
    Position finval;
public:
    ConcStream (Concordance *conc, bool useview = false,
                ConcIndex beg = 0, ConcIndex end = 0);
    virtual ~ConcStream () {}
    virtual bool next () {return ++curr < endidx;}
    virtual Position peek_beg () const;
    virtual Position peek_end () const;
    virtual Position find_beg (Position pos) {return peek_beg();}
    virtual Position find_end (Position pos) {return peek_beg();}
    virtual NumOfPos rest_min () const {return endidx - curr;}
    virtual NumOfPos rest_max () const {return endidx - curr;}
    virtual Position final () const {return finval;}
    virtual int nesting() const {return conc->nesting();}
    virtual bool epsilon() const {return false;}
    virtual void add_labels (Labels &lab) const;
    virtual bool end() const {return curr >= endidx;}
protected:
    virtual Position get_curr () const;
};

ConcStream::ConcStream (Concordance *conc, bool useview,
                        ConcIndex beg, ConcIndex end)
    : conc (conc), useview (useview && conc->view), finval (conc->corp->size()),
      curr (beg), endidx (end)
{
    if (!endidx || endidx > conc->size())
        endidx = conc->size();
    if (useview && endidx > conc->viewsize())
        endidx = conc->viewsize();
}

Position ConcStream::peek_beg () const
{
    return curr >= endidx ? finval : conc->beg_at (get_curr());
}

Position ConcStream::peek_end () const
{
    return curr >= endidx ? finval : conc->end_at (get_curr());
}

void ConcStream::add_labels (Labels &lab) const
{
    for (int i = 0; i < conc->numofcolls(); i++) {
        lab[i + 1] = conc->coll_beg_at (i + 1, get_curr());
        lab[-i - 1] = conc->coll_end_at (i + 1, get_curr());
    }
    if (conc->linegroup)
        lab[Concordance::lngroup_labidx] = conc->get_linegroup (get_curr());
}

inline Position ConcStream::get_curr () const
{
    if (useview) return (*conc->view)[curr];
    return curr;
}

RangeStream * Concordance::RS (bool useview, ConcIndex beg, ConcIndex end)
{
    return new ConcStream (this, useview, beg, end);
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
