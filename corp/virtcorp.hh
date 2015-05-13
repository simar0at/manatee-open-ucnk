//  Copyright (c) 2004-2013  Pavel Rychly

#ifndef VIRTCORP_HH
#define VIRTCORP_HH

#include <finlib/fstream.hh>
#include <finlib/range.hh>
#include <vector>
#include <string>

class Corpus;
class PosAttr;

class VirtualCorpus {
public:
    struct PosTrans {
        // transition from original positions to new positions,
        // the size of the included region is computed as a difference of
        // next newpos and current newpos (in the postrans array)
        Position orgpos;
        Position newpos;
        PosTrans (Position o, Position n) : orgpos(o), newpos(n){}
    };
    struct Segment {
        Corpus *corp;
        // last item in postrans indicates the end of the last
        // transition region and the start position of the next segment
        std::vector<PosTrans> postrans;
    };
    std::vector<Segment> segs;
    //string path;

    virtual Position size() {return segs.back().postrans.back().newpos;}
    //VirtualCorpus(const string &path) : path(path){}
};

VirtualCorpus* setup_virtcorp (const std::string &filename);
PosAttr* setup_virtposattr (VirtualCorpus *vc, const std::string &path,
                            const std::string &name, bool ownedByCorpus=true);
ranges* setup_virtstructrng (VirtualCorpus *vc, const std::string &name);
VirtualCorpus* virtcorp2virtstruc (VirtualCorpus *vc, const std::string &name);

#endif

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
