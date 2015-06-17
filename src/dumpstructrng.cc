#include <corpus.hh>
#include <finlib/fromtof.hh>
#include <iostream>
#include <stdint.h>

int main (int argc, const char **argv) {
    if (argc < 4) {
        cerr << "Usage: dumpstructrng CORPUS STRUCT OUTFILE\n"
             << "Dumps begin-end ranges of (virtual) corpus structure\n"
             << "CORPUS    corpus configuration file\n"
             << "STRUCT    corpus structure\n"
             << "OUTFILE   output file, use '-' for stdout\n";
        return 1;
    }
    Corpus c (argv[1]);
    ranges *rng = c.get_struct (argv[2])->rng;
    ToFile<int64_t> *out;
    if (*argv[3] == '-')
        out = new ToFile<int64_t> (stdout);
    else
        out = new ToFile<int64_t> (argv[3]);
    for (NumOfPos num = 0; num < rng->size(); num++) {
        out->put (rng->beg_at (num));
        out->put (rng->end_at (num));
    }
    delete out;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
