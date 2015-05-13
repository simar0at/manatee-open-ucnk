//  Copyright (c) 2012-2013  Pavel Rychly

#include "corpus.hh"
#include "virtcorp.hh"
#include <iostream>

using namespace std;


class VirtualRanges: public ranges
{
public:
    struct Trans {
        // transition from original positions to new positions,
        // the size of the included region is computed as a difference of
        // next newpos and current newpos (in the postrans array)
        Position orgpos;
        Position newpos;
        // transition of structure numbers
        NumOfPos orgnum;
        NumOfPos newnum;
        Trans () {}
        Trans (Position op, Position np, NumOfPos on, NumOfPos nn) 
            : orgpos(op), newpos(np), orgnum(on), newnum(nn) {}
    };

    struct Segment {
        ranges *src;
        vector<Trans> *postrans;
        Segment(): src(NULL), postrans(NULL) {}
        ~Segment() {
            delete src;
        }
    };
    vector<Segment> segs;
    Position finval;
    inline Position org2newpos (Position pos, size_t sg, size_t tr) {
        Trans &t  = (*segs[sg].postrans)[tr];
        return pos - t.orgpos + t.newpos;
    }
    inline Position new2orgpos (Position pos, size_t sg, size_t tr) {
        Trans &t  = (*segs[sg].postrans)[tr];
        return pos - t.newpos + t.orgpos;
    }
    // original position inside the sg/tr transition
    inline bool intrans (Position pos, size_t sg, size_t tr) {
        Trans *t  = &(*segs[sg].postrans)[tr];
        return pos >= t->orgpos && pos < t->orgpos - t->newpos + (t+1)->newpos;
    }
public:
    VirtualRanges (VirtualCorpus *vc, const string &name)
        : segs (vc->segs.size()), finval (vc->size())
    {
        NumOfPos lastnum = 0;
        for (size_t i = 0; i < segs.size(); i++) {
            Segment* s=&segs[i];
            s->src = vc->segs[i].corp->get_struct(name)->rng;
            vector<VirtualCorpus::PosTrans> &pt = vc->segs[i].postrans;
            s->postrans = new vector<Trans>(pt.size());
            for (size_t j = 0; j < pt.size(); j++) {
                Trans &t = (*s->postrans)[j];
                t.orgpos = pt[j].orgpos;
                t.newpos = pt[j].newpos;
                if (j+1 < pt.size()) {
                    t.orgnum = s->src->num_at_pos(t.orgpos);
                    NumOfPos tonum = s->src->num_at_pos(t.orgpos - t.newpos
                                                        + pt[j+1].newpos);
                    t.newnum = lastnum;
                    lastnum += tonum - t.orgnum;
                } else {
                    // last transition
                    t.orgnum = 100000000000LL;
                    t.newnum = lastnum;
                }
            }
        }
    }
    virtual ~VirtualRanges() {}
protected:
    Position locate_tran_pos (Position pos, int &segnum, int &trannum) {
        // locate transition containing newpos pos, returns original position
        // XXX don't start from 0 for big postrans vectors
        segnum = trannum = 0;
        while (segnum < segs.size() && 
               pos >= segs[segnum].postrans->back().newpos)
            segnum++;
        if (segnum >= segs.size())
            return -1;
        while (trannum < segs[segnum].postrans->size() -1 &&
               pos >= (*segs[segnum].postrans)[trannum +1].newpos)
            trannum++;
        return pos - (*segs[segnum].postrans)[trannum].newpos 
            + (*segs[segnum].postrans)[trannum].orgpos;
    }

    Position locate_tran_num (NumOfPos idx, int &segnum, int &trannum) {
        // locate transition containing newpos pos, returns original position
        // XXX don't start from 0 for big postrans vectors
        segnum = trannum = 0;
        while (segnum < segs.size() && 
               idx >= segs[segnum].postrans->back().newnum)
            segnum++;
        if (segnum >= segs.size())
            return -1;
        while (trannum < segs[segnum].postrans->size() -1 &&
               idx >= (*segs[segnum].postrans)[trannum +1].newnum)
            trannum++;
        return idx - (*segs[segnum].postrans)[trannum].newnum
            + (*segs[segnum].postrans)[trannum].orgnum;
    }


    class WholeRStream: public RangeStream {
        VirtualRanges *vr;
        int sg, tr;
        RangeStream *curr;
        void locate () {
            do {
                if (! curr || curr->end() 
                    || tr >= int(vr->segs[sg].postrans->size())) {
                    delete curr;
                    if (++sg >= vr->segs.size()) {
                        curr = NULL;
                        break;
                    }
                    curr = vr->segs[sg].src->whole();
                    tr = 0;
                } else if (! vr->intrans(curr->peek_beg(), sg, tr))
                    ++tr;
                curr->find_beg((*vr->segs[sg].postrans)[tr].orgpos);
            } while (! vr->intrans (curr->peek_beg(), sg, tr));
        }
    public:
        WholeRStream (VirtualRanges *vr) 
            : vr(vr), sg(0), tr(0), curr (vr->segs[0].src->whole()) {
            locate();
        }
        ~WholeRStream () {}
        virtual bool next () {
            if (! curr)
                return false;
            curr->next();
            locate();
            return curr != NULL;
        }
        virtual Position peek_beg () const {
            if (! curr)
                return vr->finval;
            return vr->org2newpos (curr->peek_beg(), sg, tr);
        }
        virtual Position peek_end () const {
            if (! curr)
                return vr->finval;
            return vr->org2newpos (curr->peek_end(), sg, tr);
        }
        virtual void add_labels (Labels &lab) const {curr->add_labels(lab);}
        virtual Position find_beg (Position pos) {
            if (! curr)
                return vr->finval;
            // XXX do not delete
            delete curr;
            curr = NULL;
            if (vr->locate_tran_pos (pos, sg, tr) < 0)
                return vr->finval;
            locate();
            if (! curr)
                return vr->finval;
            return vr->org2newpos (curr->peek_beg(), sg, tr);
        }
        virtual Position find_end (Position pos) {
            if (! curr)
                return vr->finval;
            // XXX do not delete, locate sg/tr after find_end
            delete curr;
            if (vr->locate_tran_pos (pos, sg, tr) < 0) {
                curr = NULL;
                return vr->finval;
            }
            curr = vr->segs[sg].src->whole();
            --tr;
            locate();
            return vr->org2newpos (curr->peek_beg(), sg, tr);
        }
        virtual NumOfPos rest_min () const {return 0;}
        virtual NumOfPos rest_max () const {return curr->rest_max();} //XXX sum
        virtual Position final () const {return vr->finval;}
        virtual int nesting () const {return 0;} // nested structures not supported
        virtual bool epsilon () const {return false;}
    };



public:
    virtual NumOfPos size () {return segs.back().postrans->back().newnum;}

    virtual Position beg_at (NumOfPos idx) {
        int sg=0, tr=0;
        Position n = locate_tran_num (idx, sg, tr);
        if (n < 0)
            return finval;
        return segs[sg].src->beg_at(n) - (*segs[sg].postrans)[tr].orgpos
            + (*segs[sg].postrans)[tr].newpos; 
    }
    virtual Position end_at (NumOfPos idx) {
        int sg=0, tr=0;
        Position n = locate_tran_num (idx, sg, tr);
        if (n < 0)
            return finval;
        return segs[sg].src->end_at(n) - (*segs[sg].postrans)[tr].orgpos
            + (*segs[sg].postrans)[tr].newpos;
    }
    virtual NumOfPos num_at_pos (Position pos) {
        int sg=0, tr=0;
        Position p = locate_tran_pos (pos, sg, tr);
        if (p < 0)
            return size(); // XXX ??
        return segs[sg].src->num_at_pos(p) - (*segs[sg].postrans)[tr].orgnum
            + (*segs[sg].postrans)[tr].newnum;
    }
    virtual NumOfPos num_next_pos (Position pos) {
        int sg=0, tr=0;
        Position p = locate_tran_pos (pos, sg, tr);
        if (p < 0)
            return size(); // XXX ??
        Position x = segs[sg].src->num_next_pos(p) - (*segs[sg].postrans)[tr].orgnum
            + (*segs[sg].postrans)[tr].newnum;
        return x;
    }
    virtual RangeStream *whole() {
        return new WholeRStream (this);
    }
    virtual RangeStream *part(FastStream *p) {
        RangeStream *s = new WholeRStream (this);
        s->find_beg(s->final()); // empty stream
        return s; // XXX part range
    }
    virtual int nesting_at (NumOfPos idx) {
        // XXX nested structures not supported
        return 0;
    }
};




ranges* setup_virtstructrng (VirtualCorpus *vc, const string &name)
{
    return new VirtualRanges (vc, name);
}

VirtualCorpus* virtcorp2virtstruc (VirtualCorpus *vc, const std::string &name)
{
    VirtualRanges* vr = new VirtualRanges(vc, name);
    VirtualCorpus* vs = new VirtualCorpus();

    VirtualCorpus::Segment *seg = NULL;
    VirtualCorpus::Segment empty_segment;
    for (size_t i = 0; i < vr->segs.size(); i++) {
        vs->segs.push_back(empty_segment);
        seg = &vs->segs.back();
        seg->corp = vc->segs[i].corp->get_struct(name);

        vector<VirtualRanges::Trans> &pt = *(vr->segs[i].postrans);
        for (size_t j = 0; j < pt.size(); j++) {
            seg->postrans.push_back(VirtualCorpus::PosTrans(pt[j].orgnum, 
                                                            pt[j].newnum));
        }
    }
    return vs;
}
