//  Copyright (c) 2003-2004  Pavel Rychly, Milos Jakubicek

#ifndef PARSOP_HH
#define PARSOP_HH

#include <finlib/frstream.hh>
#include "posattr.hh"

class RQFilterPos: public RangeStream {
public:
    typedef enum {F_EQ, F_NEQ} Op;
protected:
    RangeStream *src;
    PosAttr *attr1, *attr2;
    Op op;
    int arg1, arg2;
    bool active;
    void locate();
public:
    RQFilterPos (RangeStream *rs, PosAttr *a1, PosAttr *a2, Op o, 
		 int arg1, int arg2);
    ~RQFilterPos();
    virtual bool next ();
    virtual Position peek_beg () const;
    virtual Position peek_end () const;
    virtual void add_labels (Labels &lab) const;
    virtual Position find_beg (Position pos);
    virtual Position find_end (Position pos);
    virtual NumOfPos rest_min () const;
    virtual NumOfPos rest_max () const;
    virtual Position final () const;
    virtual int nesting() const;
    virtual bool epsilon() const;
};

class RQFilterFreq: public RangeStream {
public:
    typedef enum {F_EQ, F_LEQ, F_GEQ} Op;
protected:
    RangeStream *src;
    PosAttr *attr1;
    Op op;
	int neg;
    int arg1;
	NumOfPos freq;
    bool active;
    void locate();
public:
    RQFilterFreq (RangeStream *rs, PosAttr *a1, Op o, int neg, int arg1, NumOfPos freq);
    ~RQFilterFreq();
    virtual bool next ();
    virtual Position peek_beg () const;
    virtual Position peek_end () const;
    virtual void add_labels (Labels &lab) const;
    virtual Position find_beg (Position pos);
    virtual Position find_end (Position pos);
    virtual NumOfPos rest_min () const;
    virtual NumOfPos rest_max () const;
    virtual Position final () const;
    virtual int nesting() const;
    virtual bool epsilon() const;
};

#endif
