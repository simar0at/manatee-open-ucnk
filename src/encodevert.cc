// Copyright (c) 1999-2014  Pavel Rychly, Milos Jakubicek

#include <finlib/fstream.hh>
#include <finlib/writelex.hh>
#include <finlib/fromtof.hh>
#include <finlib/fingetopt.hh>
#include <finlib/consumer.hh>
#include <finlib/log.hh>
#include "config.hh"
#include "corpconf.hh"
#include <sstream>
#include <iostream>
#include <fstream>
#if defined __MINGW32__
#include <pcreposix.h>
#else
#include <regex.h>
#endif
#include <unistd.h>
#if defined MANATEE_DARWIN
#include <sys/sysctl.h>
#endif
#include <limits>
#include <stdexcept>

#define MAX_NESTING 100

using namespace std;

class write_region;
typedef map<string,write_region*> MWR;

unsigned long long int linenum;
const char *infilename;
MWR *show_structs = NULL;
bool append_opt = false;
long int bucket_size = 10000;
long int max_memory = 0;
vector<string> dynattrs;
bool skip_regexopt = false;
string corpname = "";

const char *fileline();

const char *my_basename (const char *path)
{
    const char *s = strrchr (path, '/');
    return (s ? s+1 : path);
}

FILE *try_popen (const string &cmd, const char *type)
{
    FILE *f = popen (cmd.c_str(), type);
    if (!f)
        throw FileAccessError (cmd, "popen");
    else
        return f;
}

inline char *my_strtok (char *&start, char delim, bool &eos)
{
    if (!*start) {
        eos = true;
        return start;
    }
    eos = false;
    char *ret = start;
    char *d = strchr (start, delim);
    static char emptystr[1] = "";
    if (!d)
        start = emptystr;
    else {
        *d = '\0';
        start = d +1;
    }
    return ret;
}

inline void ensuresize (char *&ptr, int &allocsize, int needed, 
                        const char *where)
{
    if (allocsize >= needed) return;
    char *p = (char *)realloc (ptr, needed);
    if (!p)
        throw MemoryAllocationError (where);
    allocsize = needed;
    ptr = p;
}

TextConsumer::TextType 
type2consumer (const string &typestr, TextConsumer::TextType default_type)
{
    if (typestr == "FD_FBD")
        return TextConsumer::tdeltabig;
    else if (typestr == "FD_FGD" || typestr == "NoMem" || typestr == "MD_MGD")
        return TextConsumer::tdeltagiga;
    else if (typestr == "MD_MI")
        return TextConsumer::tint;
    else
        return default_type;
}


long int tt_scaling (TextConsumer::TextType tt, long int *base_sizes,
                     const char *force_size="")
{
    long int base;
    switch (tt) {
    case TextConsumer::tint:
        base = base_sizes[0];
        break;
    case TextConsumer::tdeltagiga:
        base = base_sizes[2];
        break;
    default:
        base = base_sizes[1];
    }
    if (force_size[0] != '\0') {
        long int fsize = atol (force_size) * 2;
        if (fsize < 100)
            fsize = 100;
        return fsize;
    }
    return base * bucket_size;
}

// in memory lexicon size: 10.000, 100.000, 400.000
long int lex_mem[3] = {1, 10, 40};
// index sorting buffer (item=12B): 100.000, 4.000.000, 20.000.000
long int rev_mem[3] = {10, 400, 2000};

long int attr_mem_usage (CorpInfo::MSS &aopts, TextConsumer::TextType deftt)
{
    if (aopts["DYNAMIC"] != "")
        return 0;
    TextConsumer::TextType tt = type2consumer (aopts["TYPE"], deftt);
    // 20B per lexicon item of attribute, 40B in structures
    int lexitemsize = (deftt == TextConsumer::tint ? 40 : 20);
    return lexitemsize * tt_scaling (tt, lex_mem, aopts["LEXICONSIZE"].c_str())
        + tt_scaling (tt, rev_mem) * 12; // 12 B per buffer item
}

long long int estimate_mem_usage (CorpInfo *ci)
{
    long long int mem_usage = 0;
    // attributes
    for (CorpInfo::VSC::iterator i=ci->attrs.begin(); i != ci->attrs.end(); i++)
        mem_usage += attr_mem_usage((*i).second->opts, TextConsumer::tdelta);

    // attributes of structures
    for (CorpInfo::VSC::iterator s=ci->structs.begin(); 
         s != ci->structs.end(); s++) {
        CorpInfo::VSC &at = (*s).second->attrs;
        for (CorpInfo::VSC::iterator i=at.begin(); i != at.end(); i++)
            mem_usage += attr_mem_usage((*i).second->opts, TextConsumer::tint);
    }
    return mem_usage;
}

long long int available_memory ()
{
    long long int mem;
    void *x;
#if defined MANATEE_DARWIN
    int mib[2];
    size_t len;

    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE; /* physical ram size */
    len = sizeof(mem);
    sysctl(mib, 2, &mem, &len, NULL, 0);
#else
    mem = ((long long int) sysconf(_SC_PHYS_PAGES)) * sysconf(_SC_PAGE_SIZE);
#endif
    mem /= 2;
    x = malloc(mem);
    while (x == NULL) {
        mem = mem * 7 / 10;
        x = malloc(mem);
    }
    free (x);
    return mem / (1024*1024);
}

class write_attr {
public:
    string name;
protected:
    Position currpos;
    char *default_val;
public:
    write_attr (const string &filename, const char *def)
        : name (filename), 
          default_val (strdup(def)) {}
    virtual ~write_attr () {
        free(default_val);
    }
    virtual void write (char *str, const char *curr_fileline = 0) = 0;
    virtual void fill_up_to (Position pos) = 0;
    virtual void write (const char *str, Position pos, const char *curr_fileline=0) = 0;
    Position get_pos() {return currpos;}
    virtual void compile_regexopt () {
        if (!skip_regexopt && !corpname.empty()) {
            size_t slash = name.rfind('/');
            if (slash != name.npos)
                name = string (name, slash + 1);
            CERR << "Creating regular expression optimization attribute "
                 << name << endl;
            if (system (("mkregexattr " + corpname + " " + name).c_str()) != 0)
                CERR << "ERROR: failed to create regular expression attribute "
                     << name << endl;
        }
    }
};

class write_full_attr: public write_attr {
protected:
    write_lexicon *lex;
    RevFileConsumer *rev;
    TextConsumer *corp;
    bool multivalue;
    const char multisep;
public:
    write_full_attr (const string &filename,
                TextConsumer::TextType tt=TextConsumer::tdelta,
                const char *def="", bool multiv=false, const char multis=',',
                const char *lexiconsize="")
        : write_attr (filename, def),
        lex (new write_lexicon(filename, tt_scaling(tt, lex_mem, lexiconsize))),
        rev (RevFileConsumer::create (name, tt_scaling(tt, rev_mem), 1, append_opt)),
        corp (TextConsumer::create (tt, name, append_opt)),
        multivalue (multiv), multisep (multis) {
        currpos = append_opt ? corp->curr_size() : 0;
    }
    ~write_full_attr () {
        size_t lex_size = lex->size();
        delete lex; delete corp; delete rev;
        if (lex_size > 10000)
            compile_regexopt();
    }
    void write (char *str, const char *curr_fileline = 0) {
        if (!str)
            str = default_val;
        char *begin = str;
        size_t end;	
        for (; *str == ' '; str++);
        for (end = strlen(str) - 1; str[end] == ' '; end--);
        if (begin != str || end != strlen(str) - 1) {
            str[end + 1] = 0;
            if (!curr_fileline) {
                curr_fileline = fileline();
            }
            CERR << curr_fileline << "stripping space in attribute value\n";
        }
        int id = lex->str2id (str);
        corp->put (id);
        rev->put (id, currpos);
        if (multivalue) {
            if (!multisep) {
                // multisep == "" --> store characters
                if (str[0] && str[1]) {
                    char value[2];
                    value[1] = 0;
                    while (*str) {
                        value[0] = *str;
                        rev->put (lex->str2id (value), currpos);
                    }
                }
            } else if (strchr (str, multisep)) {
                static int value_len = 0;
                static char *value = NULL;
                ensuresize (value, value_len, strlen (str) +1, 
                            "write_attr::write");
                strncpy (value, str, value_len);
                char *save_ptr = value;
                bool endofval;
                for (char *s = my_strtok (save_ptr, multisep, endofval); 
                     !endofval; 
                     s = my_strtok (save_ptr, multisep, endofval)) {
                    rev->put (lex->str2id (s), currpos);
                }
            }
        }
        currpos++;
    }
    void fill_up_to (Position pos) {
        int id;
        if (currpos < pos) {
            id = lex->str2id (default_val);
            while (currpos < pos) {
                corp->put (id);
                rev->put (id, currpos++);
            }
        }
    }
    void write (const char *str, Position pos, const char *curr_fileline=0) {
        if (pos < currpos) {
            if (!curr_fileline)
                curr_fileline = fileline();
            CERR << curr_fileline
                 << "error: corrupted file (" << name 
                 << "), incorrect position: " << pos 
                 << " instead of " << currpos << '\n';
            return;
        }
        fill_up_to (pos);
        write ((char *) str, curr_fileline);
    }
};

class write_unique_attr: public write_attr {
protected:
    write_unique_lexicon *lex;
public:
    write_unique_attr (const string &filename, const char *def="")
        : write_attr(filename, def), lex (new write_unique_lexicon(filename)) {
        currpos = append_opt ? lex->size() : 0;
    }
    ~write_unique_attr () {
        /* XXX check that lex is indeed unique */
        size_t lex_size = lex->size();
        delete lex;
        if (lex_size > 10000)
            compile_regexopt();
    }
    void write (char *str, const char *curr_fileline = 0) {
        if (!str)
            str = default_val;
        char *begin = str;
        size_t end;
        for (; *str == ' '; str++);
        for (end = strlen(str) - 1; str[end] == ' '; end--);
        if (begin != str || end != strlen(str) - 1) {
            str[end + 1] = 0;
            if (!curr_fileline) {
                curr_fileline = fileline();
            }
            CERR << curr_fileline << "stripping space in attribute value\n";
        }
        lex->str2id (str);
        currpos++;
    }
    void fill_up_to (Position pos) {
        for (; currpos < pos; currpos++)
            lex->str2id (default_val);
    }
    void write (const char *str, Position pos, const char *curr_fileline=0) {
        if (pos < currpos) {
            if (!curr_fileline)
                curr_fileline = fileline();
            CERR << curr_fileline
                 << "error: corrupted file (" << name
                 << "), incorrect position: " << pos
                 << " instead of " << currpos << '\n';
            return;
        }
        fill_up_to (pos);
        write ((char *) str, curr_fileline);
    }

};

write_attr *new_write_attr (const string &filename, CorpInfo::MSS &aopts, 
                            TextConsumer::TextType deftt)
{
    if (aopts["TYPE"] == "UNIQUE") return
        new write_unique_attr (filename,
            aopts["DEFAULTVALUE"].empty() ? "===NONE===" : aopts["DEFAULTVALUE"].c_str());
    else return
        new write_full_attr (filename, type2consumer (aopts["TYPE"], deftt),
            aopts["DEFAULTVALUE"].empty() ? "===NONE===" : aopts["DEFAULTVALUE"].c_str(),
            CorpInfo::str2bool (aopts["MULTIVALUE"]),
            aopts["MULTISEP"][0], aopts["LEXICONSIZE"].c_str());
}

class int_writer {
public:
    int_writer(){}
    virtual ~int_writer(){}
    virtual void put (Position p) = 0;
    virtual void takeback (int count) = 0;
    virtual Position size() = 0;
};

template <class IntType>
class IntWriter: public int_writer
{
    ToFile<IntType> rng;
public:
    IntWriter(const string &path): rng(path, append_opt) {}
    virtual void takeback (int count) {rng.takeback(count);}
    virtual void put (Position p);
    virtual Position size() {return rng.tell();}
};

int_writer *create_int_writer (const string &filename, const string &type)
{
    if (type == "file64" || type == "map64")
        return new IntWriter<int64_t>(filename);
    else
        return new IntWriter<int32_t>(filename);
}

template <> void IntWriter<int64_t>::put (Position p) {rng(p);}
template <> void IntWriter<int32_t>::put (Position p) {
    if (p > numeric_limits<int32_t>::max())
        throw overflow_error ("Too many positions, use file64/map64");
    rng(p);
}

class write_region {
protected:
    const string path;
    const char *base;
    int_writer *rng;
    Position regnum;
    write_attr *vals;
    Position lastpos;
    bool allownested;
    struct Nested_rng {
        Position beg, end;
        string val_fileline;
        string value;
        Nested_rng (Position beg)
            : beg(beg), end(-1), val_fileline(fileline()) {}
    };
    vector<Nested_rng> notstored;

    virtual void flush_notstored() {
        bool nesteditem = false;
        for (vector<Nested_rng>::iterator i=notstored.begin(); 
             i != notstored.end(); i++) {
            rng->put(i->beg);
            rng->put(nesteditem ? -i->end : i->end);
            nesteditem = true;
            regnum++;
            if (!i->value.empty())
                store_val (i->value.c_str(), i->val_fileline);
        }
        notstored.clear();
    }
    virtual void store_val (const char *valuestr, const string &val_fileline) {
        if (!vals)
            vals = new write_full_attr (path + ".val", TextConsumer::tint);
        vals->write (valuestr, regnum, val_fileline.c_str());
    }
public:
    write_region (const string &filename, const string &type, bool nested)
        :path (filename), base (my_basename (path.c_str())),
         rng (create_int_writer (path + ".rng", type)),
         regnum (rng->size()/2 -1), vals (NULL), lastpos (-1),
         allownested (nested) {}
    virtual ~write_region() {
        if (vals) {
            flush_notstored();
            delete vals;
        }
        delete rng;
    }
    virtual bool is_open() {
        return !notstored.empty() && notstored[0].end == -1;
    }
    virtual void print_opened (ostringstream &out, const string &name) {
        for (size_t i=0; i < notstored.size(); i++) {
            if (notstored[i].end == -1)
                out << "  inside <" << name << " " 
                    << notstored[i].value << ">\n";
        }
    }
    virtual void open (Position pos) {
        if (!allownested) {
            if (is_open())
                notstored[0].end = pos;
            if (lastpos == pos) {
                CERR << fileline() << "warning: opening structure (" << base
                     << ") on the same position, ignoring "
                    "the previous empty one\n";
                notstored.pop_back();
            } else {
                flush_notstored();
                lastpos = pos;
            }
        }
        if (notstored.size() >= MAX_NESTING) {
            CERR << notstored[0].val_fileline
                 << "warning: closing too many nested structures (" << base
                 << "), first open structure starts here, maximum allowed nesting is "
                 << MAX_NESTING << "\n";
            while (is_open()) {
                close(pos);
            }
        }
        notstored.push_back(Nested_rng(pos));
    }
    virtual void open (Position pos, const char *valuestr) {
        open (pos);
        notstored.back().value = valuestr;
    }
    virtual void close (Position pos, int warn = 0) {
        if (!is_open()) {
            CERR << fileline() << "warning: closing non opened structure ("
                 << base << ")\n";
            return;
        }
        for (int i = notstored.size() -1; i >= 0; i--)
            if (notstored[i].end == -1) {
                notstored[i].end = pos;
                if (warn) {
                    CERR << fileline()
                         << "warning: auto-closing structure '" << base << "'\n"
                         << notstored[i].val_fileline << "   ... opened here\n";
                }
                break;
            }
        if (allownested && !is_open())
            flush_notstored();
    }
};

static const char *read_literal (const char *txtin, char *literal, 
                                 int lit_size)
{
    while ((isalnum (*txtin) || *txtin == '_') && --lit_size > 0)
        *(literal++) = *(txtin++);
    *literal = 0;
    return txtin;
}

static const char *read_string (const char *txtin, char *str, char enclosed,
                                int str_size)
{
    while (*txtin && *txtin != enclosed && --str_size > 0) {
        if (*txtin == '\\' && (txtin[1] == enclosed || txtin[1] == '\\'))
            txtin++;
        *(str++) = *(txtin++);
    }
    txtin++;
    *str = 0;
    return txtin;
}

static const char *skip_white (const char *txtin)
{
    while (isspace (*txtin))
        txtin++;
    return txtin;
}

static write_attr *ignored_attr = (write_attr*)1;

class write_region_attr : public write_region {
    typedef map<string,write_attr*> MSW;
    MSW attrs;
    CorpInfo *structci;
public:
    write_region_attr (const string &filename, CorpInfo *strci = NULL) 
        : write_region (filename, strci ? strci->opts["TYPE"] : "", 
                        strci ? 
                        CorpInfo::str2bool(strci->opts["NESTED"]) : false),
          structci (strci) {}
    virtual ~write_region_attr () {
        if (structci) {
            // add missing attributes present in configuration
            for (CorpInfo::VSC::iterator i = structci->attrs.begin();
                 i != structci->attrs.end(); i++) {
                CorpInfo::MSS &ao = (*i).second->opts;
                if (ao["DYNAMIC"] != "") {
                    // store dynamic attributes for postprocessing
                    if (ao["DYNTYPE"] != "plain")
                        dynattrs.push_back (base + ("." + (*i).first));
                    continue;
                }
                if (!attrs[(*i).first]) {
                    attrs[(*i).first] = new_write_attr(path + '.' + (*i).first,
                                        structci->find_attr ((*i).first),
                                        TextConsumer::tint);
                    CERR << "warning: structure attribute (" << base << "."
                         << (*i).first << ") never present\n";
                }
            }
        }
        flush_notstored();
        for (MSW::iterator i=attrs.begin(); i != attrs.end(); i++) {
            if ((*i).second == ignored_attr)
                continue;
            (*i).second->fill_up_to (regnum +1);
            delete (*i).second;
        }
    }
    virtual void store_val (const char *valuestr, const string &val_fileline) {
        const char *s = valuestr;
        char attr_name [80];
        static char *value = NULL;
        static int value_len = 0;
        ensuresize (value, value_len, strlen (valuestr) +1, 
                    "write_region_attr::store_val");
        while (*s) {
            s = read_literal (s, attr_name, sizeof (attr_name));
            if (attr_name[0] == 0) {
                CERR << val_fileline << "empty attribute name\n";
                break;
            }
            s = skip_white (s);
            if (*s != '=') {
                CERR << val_fileline << "expecting `=', got `" << *s << "'\n";
                break;
            }
            s++;
            s = skip_white (s);
            if (*s == '\'' || *s == '"')
                s = read_string (s+1, value, *s, value_len);
            else
                s = read_literal (s, value, value_len);
            write_attr *a = attrs [attr_name];
            if (a == NULL) {
                if (structci) {
                    try {
                        a = new_write_attr (path + '.' + attr_name, 
                                            structci->find_attr (attr_name),
                                            TextConsumer::tint);
                    } catch (CorpInfoNotFound &err) {
                        CERR << val_fileline << "structure attribute ("
                             << err.name << ") not defined, ignoring\n";
                        a = ignored_attr;                       
                    }
                } else {
                    a = new write_full_attr (path + '.' + attr_name,
                                        TextConsumer::tint);
                }
                attrs [attr_name] = a;
            }
            /* XXX subst_ent & iconv (value) */
            if (a != ignored_attr)
                a->write (value, regnum, val_fileline.c_str());
            s = skip_white (s);
        }
    }    
};


FILE* open_source (const char *in_name, CorpInfo *ci)
{
    if (in_name == NULL && ci && !ci->opts ["VERTICAL"].empty()) {
        in_name = ci->opts ["VERTICAL"].c_str();
    } 
    if (in_name == NULL || (in_name[0] == '-' && in_name[1] == '\0')) {
        infilename = "(standard input)";
        return stdin;
    } else {
        if (in_name[0] == '|') {
            FILE *p = try_popen (in_name+1, "r");
            if (p == NULL) {
                CERR << "encodevert: cannot execute command `" << in_name+1
                     << "' [" << strerror (errno) << "]\n";
                return NULL;
            }
            infilename = "(pipe)";
            return p;
        } else {
            FILE *ret;
            if (!(ret = fopen(in_name, "r"))) {
                CERR << "encodevert: cannot open file `" << in_name
                     << "' [" << strerror (errno) << "]\n";
            }
            infilename = in_name;
            return ret;
        }
    }
}

#define LINE_ALLOC_DELTA 1024
char* getline(FILE* input, char* line, size_t line_avail, size_t *line_alloc) {
    if (!fgets(line + *line_alloc - line_avail, line_avail, input)) {
        return NULL;
    } else if (line[strlen(line) - 1] != '\n' && strlen(line) == *line_alloc - 1) {
        *line_alloc += LINE_ALLOC_DELTA;
        line = (char*) realloc(line, *line_alloc);
        if (!line) {
            perror ("Out of memory.\n");
            return NULL;
        }
        return getline(input, line, LINE_ALLOC_DELTA + 1, line_alloc);
    } else {
        return line;
    }
}

int encode (const string &corp_name, const string &corp_path, 
            const char *attr_names, const char *struct_names, 
            const char *in_name, 
            TextConsumer::TextType text_type=TextConsumer::tdelta,
            bool use_xml=false, bool verbose=false, bool skip_dynamic=false)
{
    CorpInfo *ci = NULL;
    if (!corp_name.empty()) {
        corpname = corp_name;
        ci = loadCorpInfo (corp_name);

        // set memory limits to process input without running out of memory
        long long int mem = available_memory();
        if (max_memory == 0 || mem * 2 < max_memory) 
            // allow at most 2-times more memory than available
            max_memory = mem;
        mem = estimate_mem_usage(ci);
        CERR << "Available memory " << available_memory()
             << "MB, estimated usage " << mem / (1024 * 1024) << "MB\n";
        if (mem > max_memory * 1024 * 1024) {
            float scale = float(max_memory) * 1024 * 1024 / mem;
            bucket_size = (long int) (bucket_size * scale);
            CERR << "   limiting memory: bucket_size=" << bucket_size << "\n";
        }
    }

    string path = corp_path.empty() ? ci->opts ["PATH"] : corp_path;
    if (*(path.end() -1) == '/')
        path.erase (path.length() -1);
    struct stat dir_st;
    if (stat (path.c_str(), &dir_st) < 0) {
#if defined __MINGW32__
#  define mkdir_args
#else
#  define mkdir_args , S_IRWXU | S_IRWXG | S_IXOTH | S_IROTH
#endif
        if (mkdir (path.c_str() mkdir_args)) {
            CERR << "encodevert: cannot make directory `" << path << "'\n";
            return 1;
        }
    }
    path += '/';
    typedef vector<write_attr*> VWA;
    VWA attrs;
    
    // input file
    FILE *in_file = open_source (in_name, ci);
    if (!in_file)
        return 1;

    if (attr_names[0]) {
        // given attributes
        istringstream as (attr_names);
        string a;
        write_attr *wa;
        while (getline (as, a, ',')) {
            if (ci)
                wa = new_write_attr (path + a, ci->find_attr (a), text_type);
            else
                wa = new write_full_attr (path + a, text_type);
            attrs.push_back (wa);
        }
    } else {
        // attributes from config
        for (CorpInfo::VSC::iterator i=ci->attrs.begin(); i != ci->attrs.end();
             i++) {
            CorpInfo::MSS &ao = (*i).second->opts;
            if (ao["DYNAMIC"] != "") {
                // store dynamic attributes for postprocessing
                if (ao["DYNTYPE"] != "plain")
                    dynattrs.push_back ((*i).first);
                continue;
            }
            attrs.push_back (new_write_attr (path + (*i).first, ao, text_type));
        }
    }
    if (attrs.empty()) {
        CERR << "encodevert: no attributes specified!\n";
        return 2;
    }

    MWR structs;
    if (verbose)
        show_structs = &structs;
    regex_t *regpat = NULL;
    
    if ((struct_names && struct_names[0]) || (ci && !ci->structs.empty())) {
        string regpatstr = "^</?(";
        if (struct_names && struct_names[0]) {
            istringstream as (struct_names);
            string a;
            while (getline (as, a, ',')) {
                structs [a] = use_xml ? new write_region_attr (path + a) :
                    new write_region (path + a, "", false);
                regpatstr += a + '|';
            }
        } else {
            for (CorpInfo::VSC::iterator i=ci->structs.begin(); 
                 i != ci->structs.end(); i++) {
                string a = (*i).first;
                CorpInfo::VSC &at = (*i).second->attrs;
                if (at.size() != 1 || at[0].first != "val")
                    structs [a] = new write_region_attr (path +a, (*i).second);
                else
                    structs [a] = new write_region (path + a, "", false);
                regpatstr += a + '|';
            }
        }
        regpatstr.erase (regpatstr.length() -1);
        regpatstr += ")( +(.*))?/?(>)[ \t]*$";
        /* regmatch[0] line
         * regmatch[1] attrname
         * regmatch[2] rest of attr
         * regmatch[3] attrval
         * regmatch[4] '>'
         */
        regpat = new regex_t;
        if (regcomp (regpat, regpatstr.c_str(), REG_EXTENDED)) {
            CERR << "encodevert: regular expression compilation error\n";
            delete regpat;
            regpat = NULL;
        }
    }

    size_t line_alloc = LINE_ALLOC_DELTA;
    char* line = (char*) malloc(sizeof(char) * line_alloc);
    if (!line) {
        perror("Out of memory.\n");
        return 1;
    }
    int len;
#define NUM_OF_SUBEXPR 5
    regmatch_t regmatch[NUM_OF_SUBEXPR];
    linenum = 0;
    while ((line = getline (in_file, line, line_alloc, &line_alloc))) {
        linenum++;
        if (linenum % 1000000 == 0)
            CERR << "Processed " << linenum << " lines, "
                 << attrs[0]->get_pos() << " positions.\n";

        // strip ending whitespaces
        len = strlen(line);
        while (len > 0 && (line[len-1] == ' ' || line[len-1] == '\r' 
                           || line[len-1] == '\n'))
            len--;

        // ignore empty lines
        if (len == 0) {
            CERR << fileline() << "warning: empty line, ignoring\n";
            continue;
        }
        // ignore <! and <?
        if (use_xml && line[0] == '<' && (line[1] == '?' || line[1] == '!'))
            continue;
        line[len] = '\0';
        
        if (regpat && line[0] == '<' && 
            ! regexec (regpat, line, NUM_OF_SUBEXPR, regmatch, 0)) {
            // write structure
            string sname (line, regmatch[1].rm_so, 
                          regmatch[1].rm_eo - regmatch[1].rm_so);
            write_region *s = structs [sname];
            Position currpos = attrs[0]->get_pos();
            if (line[1] == '/')
                s->close (currpos);
            else {
                len = regmatch[4].rm_so;
                // empty structure (<g/>)
                bool empty = line [len -1] == '/';
                if (regmatch[3].rm_so != regmatch[3].rm_eo) {
                    if (empty)
                        len--;
                    line [len] = '\0';
                    s->open (currpos, line + regmatch[3].rm_so);
                } else
                    s->open (currpos);
                if (empty)
                    s->close (currpos);
            }
        } else {
            // write attributes
            char *val = line;
            bool endofline;
            for (VWA::iterator i=attrs.begin(); i != attrs.end(); i++) {
                /* XXX subst_ent & iconv (val) */
                char *s = my_strtok (val, '\t', endofline);
                (*i)->write (s);
            }
            if (val[0] != '\0')
                CERR << fileline() << "too many attributes\n";
        }
    }

    free(line);
    if (in_file != stdin)
        fclose(in_file);

    CERR << "Reading input file finished." << endl;
    Position finalpos = attrs[0]->get_pos();
    show_structs = NULL;
    for (MWR::iterator i=structs.begin(); i != structs.end(); i++) {
        CERR << "Closing structure " << (*i).first << " ...\n";
        while ((*i).second->is_open())
            (*i).second->close (finalpos, 1);
        delete (*i).second;
        CERR << "            ... finished.\n";
    }
    for (VWA::iterator i=attrs.begin(); i != attrs.end(); i++) {
        CERR << "Closing attribute " << (*i)->name << " ...\n";
        delete (*i);
        CERR << "            ... finished.\n";
    }

    CERR << "Processed " << linenum << " lines, "
         << finalpos << " positions.\n";


    // create dynamic attributes
    if (!skip_dynamic) {
        for (vector<string>::iterator i=dynattrs.begin();
             i != dynattrs.end(); i++) {
            CERR << "Creating dynamic attribute " << *i << endl;
            if (system (("mkdynattr " + corp_name + " " + *i).c_str()) != 0)
                CERR << "ERROR: failed to create dynamic attribute "
                     << *i << endl;
        }
    }
    return 0;
}

void usage (const char *progname) {
    cerr << "Usage: " << progname << " [OPTIONS] [FILENAME]\n"
         << "Creates a new corpus from a vertical text in file FILENAME or stdin.\n\n"
         << "    -c CORPNAME   corpus name in registry or corpus configuration file path\n"
         << "    -p CORPDIR    path to the corpus directory\n"
         << "    -v            verbose messages\n"
         << "    -e            append data to the end of an existing corpus\n"
         << "    -m MEM        use at most MEM MB of memory\n"
         << "    -d            do NOT compile dynamic attributes\n"
         << "    -r            do NOT compile regular expression optimization attributes\n"
         << "Options below apply only if configuration file is not supplied via -c option\n"
         << "    -b            use `FD_FGD' attribute type\n"
         << "                  Use this option when creating corpora over ca 250M tokens\n"
         << "                  (encodevert will stop processing if this option is necessary and was not specified)\n"
         << "    -x            use XML style of structures\n"
         << "    -a ATTRS      comma delimited list of attributes\n"
         << "    -s STRUCTS    comma delimited list of structures\n"
         << "\nExamples:\n"
         << "   encodevert -c susanne susanne.vert\n"
         << "   encodevert -a word,lemma -x -s doc,p -a tag -p /nlp/corp/desam desam.vert\n"
         << "   cat *.src | encodevert -a word,lemma,tag,status -s doc,p,head,g -p susanne\n"
        ;
}

int main (int argc, char **argv)
{
    const char *input_name = NULL;
    const char *progname = argv[0];
    const char *corpdir = NULL;
    const char *corpname = NULL;
    string oattrs = "";
    string ostructs = "";
    bool ob = false;
    bool ox = false;
    bool ov = false;
    bool od = false;
    {
        // process options
        int c;
        while ((c = getopt (argc, argv, "c:a:s:p:bxvedrh?m:")) != -1)
            switch (c) {
            case 'c':
                corpname = optarg;
                break;
            case 'p':
                corpdir = optarg;
                break;
            case 'a':
                oattrs = oattrs.empty() ? optarg : oattrs + ',' + optarg;
                break;
            case 's':
                ostructs = ostructs.empty() ? optarg : ostructs + ',' + optarg;
                break;
            case 'b':
                ob = true;
                break;
            case 'x':
                ox = true;
                break;
            case 'v':
                ov = true;
                break;
            case 'e':
                append_opt = true;
                break;
            case 'm':
                max_memory = atol(optarg);
                if (max_memory < 10)
                    max_memory = 10;
                break;
            case 'd':
                od = true;
                break;
            case 'r':
                skip_regexopt = true;
                break;
            case '?':
            case 'h':
                usage (progname);
                return 2;
            default:
              abort ();
            }
        
        if (optind < argc) {
            input_name = argv [optind++];
            if (optind < argc) {
                usage (progname);
                return 2;
            }
        }
    }
    if (oattrs.empty() && corpname == NULL) {
        CERR << "You have to specify corpus name or at least one attribute!\n";
        usage (progname);
        return 2;
    }
    if (corpdir == NULL && corpname == NULL) {
        CERR << "You have to specify corpus name or corpus directory!\n";
        usage (progname);
        return 2;
    }

    try {
        if (corpdir == NULL) corpdir = "";
        if (corpname == NULL) corpname = "";
        return encode (corpname, corpdir, oattrs.c_str(), ostructs.c_str(), 
                       input_name, 
                       (ob ? TextConsumer::tdeltagiga : TextConsumer::tdelta), 
                       ox, ov, od);
    } catch (exception &ex) {
        CERR << "encodevert error: " << ex.what() << '\n';
        return 1;
    }
}

const char *fileline()
{
    static string buff;
    ostringstream out;
    if (show_structs) {
        for (MWR::iterator i=show_structs->begin(); 
             i != show_structs->end(); i++) {
            if ((*i).second->is_open())
                (*i).second->print_opened (out, (*i).first);
        }
    }
    out << infilename << ":" << linenum << ": ";
    buff = out.str();
    return buff.c_str();
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

