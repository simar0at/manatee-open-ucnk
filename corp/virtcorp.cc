//  Copyright (c) 2004-2014  Pavel Rychly, Milos Jakubicek

#include "corpus.hh"
#include "virtcorp.hh"
#include <finlib/lexicon.hh>
#include <finlib/regexplex.hh>
#include <finlib/log.hh>
#include <fstream>
#include <iostream>

using namespace std;



VirtualCorpus* setup_virtcorp (const string &filename)
{
    VirtualCorpus* vc = new VirtualCorpus();
    VirtualCorpus::Segment *seg = NULL;
    VirtualCorpus::Segment empty_segment;
    Position lastpos = 0;
    ifstream in_file (filename.c_str());
    if (in_file.fail())
        throw FileAccessError (filename, ": could not open file");
    string line;
    while (getline (in_file, line)) {
	if (line.length() == 0 || line[0] == '#')
	    continue;
	if (line[0] == '=') {
	    // new corpus -> new segment
            if (lastpos)
                // close previous segment
                seg->postrans.push_back(VirtualCorpus::PosTrans(100000000000LL,
                                                                lastpos));
            vc->segs.push_back(empty_segment);
            seg = &vc->segs.back();
            line.erase(0,1);
            seg->corp = new Corpus (line);
        } else {
            if (! seg) {
                CERR << filename << ": transition without corpus:" << line << '\n';
                continue;
            }
            string::size_type cidx = line.find(',');
	    if (cidx == line.npos) {
                CERR << filename << ": expecting `,': " << line << '\n';
                continue;
            }
            Position fromp = atoll (string (line, 0, cidx).c_str());
            string to (line, cidx +1);
            Position top;
            NumOfPos size = seg->corp->size();
            if (to == "$")
                top = size;
            else {
                top = atoll (to.c_str());
                if (top > size) {
                    CERR << filename << ": transition exceeds corpus size: "
                         << line << "\n-- using corpus size (" << size
                         << ") instead.\n";
                    top = size;
                }
            }
            if (fromp >= top) {
                CERR << filename << ": empty transition: " << line << '\n';
                continue;
            }
            seg->postrans.push_back(VirtualCorpus::PosTrans(fromp, lastpos));
            lastpos += top - fromp;
        }
    }
    if (lastpos)
        // close last segment
        seg->postrans.push_back(VirtualCorpus::PosTrans(100000000000LL, 
                                                        lastpos));
    else
        CERR << filename << ": empty virtual corpus\n";
    return vc;
}


static const char* n2str (int num, const char *suffix)
{
    static char str[16];
    sprintf (str, ".seg%i%s", num, suffix);
    return str;
}

template <class NormClass=MapBinFile<int64_t>,
          class FreqClass=MapBinFile<uint32_t>,
          class FloatFreqClass=MapBinFile<float> >
class VirtualPosAttr : public PosAttr
{
protected:
    struct Segment {
        PosAttr *src;
        MapBinFile<int> *toorgid, *tonewid;
        const vector<VirtualCorpus::PosTrans> *postrans;
        bool ownedByCorpus;
        Segment(): src(NULL), toorgid(NULL), tonewid(NULL),
                   postrans(NULL), ownedByCorpus (false) {}
        ~Segment() {
            if (!ownedByCorpus) {
                delete src;
            }
            delete toorgid;
            delete tonewid;
        }
    };
    map_lexicon lex;
    vector<Segment> segs;
    MapBinFile<int64_t> freqs;
    friend class CombineFS;
    friend class IDIter;
    NormClass *normf;
    FreqClass *docff;
    FloatFreqClass *arff, *aldff;

public:
    VirtualPosAttr (const string &path, const string &name, 
                    const vector<VirtualCorpus::Segment> &corpsegs,
                    const string &locale, const string &enc,
                    bool ownedByCorpus = true)
        : PosAttr (path, name, locale, enc), lex (path), segs(corpsegs.size()),
          freqs (path + ".frq"), normf (NULL), docff (NULL), arff (NULL),
          aldff (NULL)
    {
        for (size_t i = 0; i < segs.size(); i++) {
            Segment* s=&segs[i];
            s->src = corpsegs[i].corp->get_attr(name);
            s->toorgid = new MapBinFile<int32_t>(path + n2str(i, ".oid"));
            s->tonewid = new MapBinFile<int32_t>(path + n2str(i, ".nid"));
            s->postrans = &corpsegs[i].postrans;
            s->ownedByCorpus = ownedByCorpus;
        }
        try {
            normf = new NormClass (path + ".norm");
        } catch (FileAccessError) {}
        try {
            docff = new FreqClass (path + ".docf");
        } catch (FileAccessError) {}
        try {
            arff = new FloatFreqClass (path + ".arf");
        } catch (FileAccessError) {}
        try {
            aldff = new FloatFreqClass (path + ".aldf");
        } catch (FileAccessError) {}
    }
    virtual ~VirtualPosAttr()
        {delete normf; delete docff; delete arff; delete aldff;}
protected:
    Position locate_tran (Position pos, size_t &segnum, size_t &trannum) {
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
    class IDIter: public IDIterator {
        VirtualPosAttr *vpa;
        IDIterator *curr;
        NumOfPos toread; //how many positions to the end of current transition
        size_t sg, tr;
    public:
        IDIter (VirtualPosAttr *vpa, Position pos) 
            : vpa (vpa), curr(NULL), toread(0) {
            Position org = vpa->locate_tran (pos, sg, tr);
            if (org >= 0) {
                curr = vpa->segs[sg].src->posat (org);
                toread = (*vpa->segs[sg].postrans)[tr+1].newpos - pos;
            }
        }
        virtual ~IDIter() {
            if (curr) delete curr;
        }
        virtual int next() {
            if (!curr) return -1;
            if (!toread) {
                // next transition
                delete curr;
                if (++tr == vpa->segs[sg].postrans->size() -1) {
                    // next segment
                    if (++sg == vpa->segs.size()) {
                        // end
                        curr = 0;
                        return -1;
                    }
                    tr = 0;
                }
                curr = vpa->segs[sg].src->posat 
                    ((*vpa->segs[sg].postrans)[tr].orgpos);
                toread = (*vpa->segs[sg].postrans)[tr+1].newpos
                    - (*vpa->segs[sg].postrans)[tr].newpos;
            }
            toread--;
            return (*vpa->segs[sg].tonewid)[curr->next()];
        }
    };
    class TextIter: public TextIterator {
        VirtualPosAttr *vpa;
        TextIterator *curr;
        NumOfPos toread; //how many positions to the end of current transition
        size_t sg, tr;
    public:
        TextIter (VirtualPosAttr *vpa, Position pos) 
            : vpa (vpa), curr(NULL), toread(0) {
            Position org = vpa->locate_tran (pos, sg, tr);
            if (org >= 0) {
                curr = vpa->segs[sg].src->textat (org);
                toread = (*vpa->segs[sg].postrans)[tr+1].newpos - pos;
            }
        }
        virtual ~TextIter() {
            if (curr) delete curr;
        }
        virtual const char *next() {
            if (!curr) return "";
            if (!toread) {
                // next transition
                delete curr;
                if (++tr == vpa->segs[sg].postrans->size() -1) {
                    // next segment
                    if (++sg == vpa->segs.size()) {
                        // end
                        curr = NULL;
                        return "";
                    }
                    tr = 0;
                }
                curr = vpa->segs[sg].src->textat 
                    ((*vpa->segs[sg].postrans)[tr].orgpos);
                toread = (*vpa->segs[sg].postrans)[tr+1].newpos
                    - (*vpa->segs[sg].postrans)[tr].newpos;
            }
            toread--;
            return curr->next();
        }
    };
    class CombineFS: public FastStream {
        VirtualPosAttr *vpa;
        vector<FastStream*> srcs; // for each segment one FastStream*
        size_t sg, tr;
        NumOfPos currdelta;
        Position tranend;
        void locate () {
            Position pe, org, trsize;
            while (sg < srcs.size()) {
                pe = srcs[sg]->peek();
                //if (pe < tranend)
                //    break;
                if (tr >= vpa->segs[sg].postrans->size() -1 // transition end
                    || pe >= srcs[sg]->final()) { // empty stream
                    // forward to next segment
                    sg++;
                    tr = 0;
                    tranend = -1;
                } else if (pe >= (*vpa->segs[sg].postrans)[tr+1].orgpos)
                    // forward to next transition
                    tr++;
                else if (pe < (org=(*vpa->segs[sg].postrans)[tr].orgpos))
                    // advance source FastStream
                    srcs[sg]->find(org);
                else if (pe >= ((trsize=(*vpa->segs[sg].postrans)[tr+1].newpos
                                 - (*vpa->segs[sg].postrans)[tr].newpos)
                                + org))
                    // forward to next transition
                    tr++;
                else {
                    // peek is inside current transition
                    currdelta = (*vpa->segs[sg].postrans)[tr].newpos - org;
                    tranend = org + trsize;
                    break;
                }
            }
        }
    public:
        CombineFS (VirtualPosAttr *vpa, const vector<FastStream*> &src) 
            : vpa (vpa), srcs (src), sg (0), tr (0), tranend(-1) {
            locate();
        }
        ~CombineFS () {
            for (sg=0; sg < srcs.size(); sg++) 
                delete srcs[sg];
        }
        virtual Position peek() {
            if (sg >= srcs.size())
                return vpa->size();
            return srcs[sg]->peek() + currdelta;
        }
        virtual Position next() {
            if (sg >= srcs.size())
                return vpa->size();
            Position ret = srcs[sg]->next() + currdelta;
            locate();
            return ret;
        }
        virtual Position find (Position pos) {
            if (sg >= srcs.size())
                return vpa->size();
            Position org = vpa->locate_tran(pos, sg, tr);
            if (org < 0)
                return vpa->size();
            srcs[sg]->find(org);
            locate();
            if (sg >= srcs.size())
                return vpa->size();
            return srcs[sg]->peek() + currdelta;
        }
        virtual NumOfPos rest_min() {
            NumOfPos ret = 0;
            for (size_t i = sg; i < srcs.size(); i++)
                ret += srcs[sg]->rest_min();
            return ret;
        }
        virtual NumOfPos rest_max() {
            NumOfPos ret = 0;
            for (size_t i = sg; i < srcs.size(); i++)
                ret += srcs[sg]->rest_max();
            return ret;
        }
        virtual Position final() {
            return vpa->size();
        }
    };
public:
    virtual int id_range () {return lex.size();}
    virtual const char* id2str (int id) {return lex.id2str (id);}
    virtual int str2id (const char *str) {return lex.str2id (str);}
    virtual int pos2id (Position pos) {
        size_t sg, tr;
        Position orgpos = locate_tran (pos, sg, tr);
        if (orgpos < 0)
            return -1;
        return (*segs[sg].tonewid)[segs[sg].src->pos2id (orgpos)];
    }
    virtual const char* pos2str (Position pos) {
        size_t sg, tr;
        Position orgpos = locate_tran (pos, sg, tr);
        if (orgpos < 0)
            return "";
        return segs[sg].src->pos2str (orgpos);
    }
    virtual IDIterator *posat (Position pos) {return new IDIter (this, pos);}
    virtual IDPosIterator *idposat (Position pos)
        {return new IDPosIterator (new IDIter (this, pos),
                                   new SequenceStream (pos, size()-1, size()));}
    virtual TextIterator *textat (Position pos) {return new TextIter (this, pos);}
    virtual FastStream *id2poss (int id) {
        vector<FastStream*> fss;
        for (size_t s = 0; s < segs.size(); s++)
            fss.push_back(segs[s].src->id2poss((*segs[s].toorgid)[id]));
        return new CombineFS (this, fss);
    }
    virtual FastStream *compare2poss (const char *pat, int cmp, bool icase) {
        vector<FastStream*> fss;
        for (size_t s = 0; s < segs.size(); s++)
            fss.push_back(segs[s].src->compare2poss(pat, cmp, icase));
        return new CombineFS (this, fss);
    }
    virtual FastStream *regexp2poss (const char *pat, bool icase) {
        vector<FastStream*> fss;
        for (size_t s = 0; s < segs.size(); s++)
            fss.push_back(segs[s].src->regexp2poss(pat, icase));
        return new CombineFS (this, fss);
    }
    virtual Generator<int> *regexp2ids (const char *pat, bool ignorecase, 
                                        const char *filter) 
        {return ::regexp2ids (lex, pat, locale, encoding, ignorecase, filter);}
    virtual NumOfPos freq (int id) {return freqs [id];}
    virtual NumOfPos docf (int id) {return docff ? (*docff)[id] : -1LL;}
    virtual float arf (int id) {return arff ? (*arff)[id] : -1LL;}
    virtual float aldf (int id) {return aldff ? (*aldff)[id] : -1LL;}
    virtual NumOfPos norm (int id) {return normf ? (*normf)[id] : freq (id);}
    virtual NumOfPos size () {return segs.back().postrans->back().newpos;}
};



PosAttr* setup_virtposattr (VirtualCorpus *vc, const string &path,
                            const string &name, const std::string &loc,
                            const std::string &enc, bool ownedByCorp)
{
    return new VirtualPosAttr<> (path, name, vc->segs, loc, enc, ownedByCorp);
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
