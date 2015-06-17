// Copyright (c) 2014 Milos Jakubicek

#include <finlib/writelex.hh>
#include <finlib/consumer.hh>
#include "posattr.hh"
#include "corpconf.hh"
#include "utf8.hh"
#include <iostream>

using namespace std;

#define NGRLENGTH 3

inline bool is_utf8char_begin (const unsigned char c)
{
    return (c & 0xc0) != 0x80;
}

int main (int argc, char **argv)
{
    if (argc < 3) {
        fputs ("usage: mkregexattr corpus attr\n", stderr);
        return 2;
    }
    try {
        CorpInfo *ci = loadCorpInfo (argv[1]);
        string attr = argv[2];
        string path = ci->opts["PATH"];
        int dotidx = attr.find ('.');
        string attr_name = attr;
        if (dotidx >= 0) {
            // struct_name.attr_name
            path = ci->find_struct (string (attr, 0, dotidx))->opts["PATH"];
            attr_name = string (attr, dotidx +1);
        }
        PosAttr *at = findPosAttr (ci, attr);
        path += attr_name + ".regex";
        delete ci;

        { // delete existing lexicons
        ToFile<int> lex (path + ".lex");
        ToFile<int> lexidx (path + ".lex.idx");
        ToFile<int> lexsrt (path + ".lex.srt");
        }
        write_lexicon wl (path, 500000);
        RevFileConsumer *rev = RevFileConsumer::create (path);

        int did, id, id_range = at->id_range();
        for (id = 0; id < id_range; id++) {
            if (at->freq(id) <= 0)
                continue;
            ostringstream oss;
            oss << '^' << at->id2str (id) << '$';
            string s = oss.str();
            size_t bytes = strlen (s.c_str());
            if (!bytes)
                continue;
            size_t len = utf8len (s.c_str());
            const char *end = s.c_str() + bytes;
            for (size_t l = 1; l <= NGRLENGTH && l <= len; l++) {
                const char *str = s.c_str();
                for (size_t i = 0; i < len - l + 1; i++) {
                    const char *in = str;
                    const char *next = NULL;
                    size_t n = l + 1;
                    while (in != end && n) {
                        if (is_utf8char_begin(*in)) {
                            n--;
                            if (n == l - 1) // position after first character
                                next = in;
                        }
                        in++;
                    }
                    char *curr_end = (char*) in - (!n);
                    char tmp_end = *curr_end;
                    *curr_end = '\0';
                    did = wl.str2id (str);
                    rev->put (did, id);
                    *curr_end = tmp_end;
                    if (!next)
                        break;
                    else
                        str = next;
                }
            }
        }
        delete rev;
        delete at;
    } catch (exception &e) {
        cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

