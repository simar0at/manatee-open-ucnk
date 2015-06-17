//  Copyright (c) 1999-2014  Pavel Rychly, Milos Jakubicek

#include "corpus.hh"
#include "config.hh"
#ifdef MANATEE_SKETCH_ENGINE
#include "ngram.hh"
#endif
#include <finlib/fromtof.hh>
#include <limits>
#include <math.h>

class StatAttr {
public:
    const string name;
    StatAttr (const string &n) : name (n) {}
    virtual NumOfPos id_range() = 0;
    virtual NumOfPos freq (NumOfPos id) = 0;
    virtual NumOfPos docf (NumOfPos id) = 0;
    virtual float arf (NumOfPos id) = 0;
    virtual float aldf (NumOfPos id) = 0;
    virtual IDPosIterator* idposat (Position pos) = 0;
};

class PosStatAttr : public StatAttr {
    PosAttr *attr;
public:
    PosStatAttr (PosAttr *a): StatAttr (a->name), attr(a) {}
    ~PosStatAttr() {}
    NumOfPos id_range() {return attr->id_range();}
    NumOfPos freq (NumOfPos id) {return attr->freq (id);}
    NumOfPos docf (NumOfPos id) {return attr->docf (id);}
    float arf (NumOfPos id) {return attr->arf (id);}
    float aldf (NumOfPos id) {return attr->aldf (id);}
    IDPosIterator* idposat (Position pos) {return attr->idposat (pos);}
};

#ifdef MANATEE_SKETCH_ENGINE
class NGramStatAttr : public StatAttr {
    NGram *ngr;
public:
    NGramStatAttr (NGram *n): StatAttr (basename (n->path.c_str())), ngr(n) {}
    ~NGramStatAttr() {delete ngr;}
    NumOfPos id_range() {return ngr->size();}
    NumOfPos freq (NumOfPos id) {return ngr->freq (id);}
    NumOfPos docf (NumOfPos id) {return ngr->docf (id);}
    float arf (NumOfPos id) {return ngr->arf (id);}
    float aldf (NumOfPos id) {return ngr->aldf (id);}
    IDPosIterator* idposat (Position pos) {return ngr->idposat (pos);}
};
#endif

struct DocFreq {
    NumOfPos freq, docnum;
    DocFreq () : freq(0), docnum(0) {}
    operator NumOfPos() const {return freq;}
};

struct RedFreq { // Reduced Frequency
    double sum;
    Position prev, first;
    RedFreq () : sum(0.0), prev(-1), first(-1) {}
    operator float() const {return sum;}
};

template <typename FreqArray, typename ValueType, typename Value64Type>
void write_freqs (NumOfPos size, const string &path, FreqArray freqs) {
    ToFile<ValueType> *freqfile = new ToFile<ValueType> (path + ".tmp");
    ToFile<Value64Type> *freq64file = NULL;
    for (NumOfPos id = 0; id < size; id++) {
        Value64Type freq = freqs [id];
        if (freq > numeric_limits<ValueType>::max()) {
            delete freqfile;
            freqfile = NULL;
            FromFile<ValueType> frq32 (path + ".tmp");
            freq64file = new ToFile<Value64Type> (path + "64.tmp");
            while (!frq32)
                freq64file->put (frq32.get());
            freq64file->put(freq);
        } else
            freqfile ? freqfile->put(freq) : freq64file->put(freq);
    }
    delete freqfile; delete freq64file; delete[] freqs;
    if (freqfile)
        rename ((path + ".tmp").c_str(), path.c_str());
    else // freq64file
        rename ((path + "64.tmp").c_str(), (path + "64").c_str());
}

StatAttr * open_attr (const char *attr, Corpus *c) {
#ifdef MANATEE_SKETCH_ENGINE
    size_t len = strlen (attr);
    if (len < 5 || strcmp(attr + len - 4, ".ngr"))
        return new PosStatAttr (c->get_attr (attr));
    return new NGramStatAttr (new NGram (c->get_conf("PATH") + attr));
#else
    return new PosStatAttr (c->get_attr (attr));
#endif
}

void Corpus::compile_aldf (const char *attr) {
    StatAttr *sa = open_attr (attr, this);
    IDPosIterator *it = sa->idposat(0);
    string path = get_conf("PATH");
    if (!get_conf("SUBCPATH").empty()) { // = is subcorpus
        path = get_conf("SUBCPATH");
        it = filter_idpos (it);
    }
    path += sa->name + ".aldf";

    RedFreq * freqs = new RedFreq[sa->id_range()](); // the () causes zero-initialization
    NumOfPos pos_count = search_size();
    double size = pos_count;
    NumOfPos prog_step, curr_prog, i = 0;
    prog_step = curr_prog = pos_count / 100;
    fprintf (stderr, "\r0 %%");
    Position last_pos = -1;
    while (!it->end()) {
        if (i > curr_prog) {
            fprintf (stderr, "\r%d %%", int (100 * i / pos_count));
            curr_prog += prog_step;
        }
        Position pos = it->peek_pos() - it->get_delta(); // recalculate position (for subcorpora)
        if (pos > last_pos) {
            i++;
            last_pos = pos;
        }
        NumOfPos id = it->peek_id();
        RedFreq *aldf = &freqs[id];
        if (aldf->prev == -1) // first hit
            aldf->first = aldf->prev = pos;
        else {
            double dn = (pos - aldf->prev) / size;
            aldf->prev = pos;
            aldf->sum += dn * log2 (dn);
        }
        it->next();
    }
    // the first/last interval is treated as a cyclic one
    for (NumOfPos id = 0; id < sa->id_range(); id++) {
        RedFreq *aldf = &freqs[id];
        if (aldf->prev == -1) // never occurred
            continue;
        double first_last = (aldf->first + size - aldf->prev) / size;
        aldf->sum += first_last * log2 (first_last);
        aldf->sum = exp2 (-aldf->sum);
    }
    fprintf (stderr, "\r100 %%\n");
    delete it;

    write_freqs<RedFreq*, float, double> (sa->id_range(), path, freqs);
}

void Corpus::compile_arf (const char *attr) {
    StatAttr *sa = open_attr (attr, this);
    IDPosIterator *it = sa->idposat(0);
    string path = get_conf("PATH");
    if (!get_conf("SUBCPATH").empty()) { // = is subcorpus
        path = get_conf("SUBCPATH");
        it = filter_idpos (it);
    }
    path += sa->name + ".arf";

    RedFreq * freqs = new RedFreq[sa->id_range()](); // the () causes zero-initialization
    NumOfPos pos_count = search_size();
    double size = pos_count;
    NumOfPos prog_step, curr_prog, i = 0;
    prog_step = curr_prog = pos_count / 100;
    fprintf (stderr, "\r0 %%");
    Position last_pos = -1;
    while (!it->end()) {
        if (i > curr_prog) {
            fprintf (stderr, "\r%d %%", int (100 * i / pos_count));
            curr_prog += prog_step;

        }
        Position pos = it->peek_pos() - it->get_delta(); // recalculate position (for subcorpora)
        if (pos > last_pos) {
            i++;
            last_pos = pos;
        }
        NumOfPos id = it->peek_id();
        double v = size / sa->freq(id); // window size
        RedFreq *arff = &freqs[id];
        if (arff->prev == -1) // first hit
            arff->first = arff->prev = pos;
        else {
            double d = pos - arff->prev;
            arff->prev = pos;
            if (d < v)
                arff->sum += d / v;
            else
                arff->sum += 1.0;
        }
        it->next();
    }
    // the first/last interval is treated as a cyclic one
    for (NumOfPos id = 0; id < sa->id_range(); id++) {
        RedFreq *arff = &freqs[id];
        if (arff->prev == -1) // never occurred
            continue;
        double v = size / sa->freq(id); // window size
        Position first_last = arff->first + size - arff->prev;
        if (first_last < v)
            arff->sum += first_last / v;
        else
            arff->sum += 1.0;
    }
    fprintf (stderr, "\r100 %%\n");
    delete it;

    write_freqs<RedFreq*, float, double> (sa->id_range(), path, freqs);
}

void Corpus::compile_frq (const char *attr) {
    StatAttr *sa = open_attr (attr, this);
    IDPosIterator *it = sa->idposat(0);
    string path = get_conf("PATH");
    if (!get_conf("SUBCPATH").empty()) { // = is subcorpus
        path = get_conf("SUBCPATH");
        it = filter_idpos (it);
    }
    path += sa->name + ".frq";

    NumOfPos * freqs = new NumOfPos[sa->id_range()](); // the () causes zero-initialization
    NumOfPos pos_count = search_size();
    NumOfPos prog_step, curr_prog, i = 0;
    prog_step = curr_prog = pos_count / 100;
    fprintf (stderr, "\r0 %%");
    Position last_pos = -1;
    while (!it->end()) {
        if (i > curr_prog) {
            fprintf (stderr, "\r%d %%", int (100 * i / pos_count));
            curr_prog += prog_step;
        }
        Position pos = it->peek_pos();
        if (pos > last_pos) {
            i++;
            last_pos = pos;
        }
        freqs[it->peek_id()]++;
        it->next();
    }
    fprintf (stderr, "\r100 %%\n");
    delete it;

    write_freqs<NumOfPos*, uint32_t, int64_t> (sa->id_range(), path, freqs);
}

void Corpus::compile_docf (const char *attr, const char *docstruc) {
    StatAttr *sa = open_attr (attr, this);
    IDPosIterator *it = sa->idposat(0);
    RangeStream *struc = get_struct (docstruc)->rng->whole();
    string path = get_conf("PATH");
    if (!get_conf("SUBCPATH").empty()) { // = is subcorpus
        path = get_conf("SUBCPATH");
        it = filter_idpos (it);
        struc = filter_query (struc);
    }
    path += sa->name + ".docf";

    DocFreq * freqs = new DocFreq[sa->id_range()](); // the () causes zero-initialization
    NumOfPos pos_count = search_size();
    NumOfPos docnum = 1;
    Position next_end = struc->peek_end();
    NumOfPos prog_step, curr_prog, i = 0;
    prog_step = curr_prog = pos_count / 100;
    fprintf (stderr, "\r0 %%");
    Position last_pos = -1;
    while (!it->end()) {
        if (i > curr_prog) {
            fprintf (stderr, "\r%d %%", int (100 * i / pos_count));
            curr_prog += prog_step;
        }
        Position pos = it->peek_pos();
        if (pos > last_pos) {
            i++;
            last_pos = pos;
        }
        if (pos >= next_end) {
            docnum++;
            struc->next();
            next_end = struc->peek_end();
        }
        DocFreq *df = &freqs[it->peek_id()];
        if (df->docnum < docnum) {
            df->freq++;
            df->docnum = docnum;
        }
        it->next();
    }
    fprintf (stderr, "\r100 %%\n");
    delete it; delete struc;

    write_freqs<DocFreq*, uint32_t, int64_t> (sa->id_range(), path, freqs);
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
