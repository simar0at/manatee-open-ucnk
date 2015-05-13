//  Copyright (c) 1999-2013  Pavel Rychly

#include "corpus.hh"
#include "dynattr.hh"
#include <finlib/binfile.hh>
#include <cstdlib>
#include <fstream>

using namespace std;

inline PosAttr *createDynAttr (CorpInfo::MSS &ao, const string &apath, 
                               const string &n, PosAttr *from,
                               bool ownedByCorpus = true)
{
    DynFun *fun = createDynFun (ao["FUNTYPE"].c_str(), 
                                ao["DYNLIB"].c_str(), ao["DYNAMIC"].c_str(),
                                ao["ARG1"].c_str(), ao["ARG2"].c_str());
    return createDynAttr (ao["DYNTYPE"], apath, n, fun, from, ao["LOCALE"],
                          CorpInfo::str2bool (ao["TRANSQUERY"]), ownedByCorpus);
}


PosAttr *Corpus::setup_attr (const string &name)
{
    try {
        PosAttr* pa;
        CorpInfo::MSS &ao = conf->find_attr (name);
        string path = conf->opts ["PATH"];
        if (ao["DYNAMIC"] != "")
            pa = createDynAttr (ao, path + name, name,
                                get_attr (ao["FROMATTR"]));
        else if (virt)
            pa = setup_virtposattr (virt, path + name, name);
        else
            pa = createPosAttr (ao["TYPE"], path + name, name, ao["LOCALE"], 
                                conf->opts["ENCODING"]);
        attrs.push_back (pair<string,PosAttr*> (name, pa));
        return pa;
    } catch (CorpInfoNotFound) {
        throw AttrNotFound (name);
    }
}

Structure *Corpus::setup_struct (const string &name)
{
    try {
        CorpInfo *ci = conf->find_struct (name);
        Structure *s;
        if (virt) {
            s = new Structure (ci, name, virt);
        } else {
            string path = conf->opts ["PATH"];
            s = new Structure (ci, path + name, name);
        }
        structs.push_back (pair<string,Structure*> (name, s));
        return s;
    } catch (CorpInfoNotFound) {
        throw AttrNotFound (name);
    }
}

void Corpus::init (CorpInfo *ci)
{
    hardcut = atol (ci->opts["HARDCUT"].c_str());
    maxctx = atol (ci->opts["MAXCONTEXT"].c_str());
    if (!ci->opts["ALIGNED"].empty()) {
        istringstream as (ci->opts["ALIGNED"]);
        string cname;
        while (getline (as, cname, ',')) {
            if (cname.empty())
                continue;
            aligned.push_back(pair<string,Corpus*>(cname, NULL));
        }
    }
    if (ci->opts["VIRTUAL"] != "")
        virt = setup_virtcorp (ci->opts["VIRTUAL"]);
}

Corpus::Corpus (CorpInfo *ci, type_t t)
    : defaultattr (NULL), virt(NULL), type(t), conf (new CorpInfo (ci))
{
    init (conf);
}

Corpus::Corpus (const string &corp_name)
    : defaultattr (NULL), virt(NULL), type(Corpus_type),
      conf (loadCorpInfo (corp_name))
{
    init (conf);
    //if (!defaultattr)
    // XXX throw (no atributes)
}

Corpus::~Corpus ()
{
    for (VSA::iterator i = attrs.begin(); i != attrs.end(); i++)
        delete (*i).second;
    for (VSS::iterator i = structs.begin(); i != structs.end(); i++)
        delete (*i).second;
    delete conf;
    if (virt)
        delete virt;
    for (int i = 0; i < aligned.size(); i++)
        delete aligned [i].second;
}

PosAttr* Corpus::get_attr (const string &name)
{
    if (name == "-")
        return get_default_attr();

    VSA::iterator i = attrs.begin();
    while (i != attrs.end()) {
        if ((*i).first == name) 
            return (*i).second;
        i++;
    }

    int j;
    if ((j = name.find ('.')) >= 0) {
        string strucname (name, 0, j);
        string attrname (name, j+1);
        return get_struct_pos_attr (strucname, attrname);
    }
    return setup_attr (name);
}

PosAttr* Corpus::get_default_attr ()
{
    if (defaultattr)
        return defaultattr;
    else
        return defaultattr = get_attr (get_conf ("DEFAULTATTR"));
}

void Corpus::set_default_attr (const string &attname)
{
    conf->opts["DEFAULTATTR"] = attname;
    defaultattr = get_attr (attname);
}


Structure* Corpus::get_struct (const string &name)
{
    VSS::iterator i = structs.begin();
    while (i != structs.end()) {
        if ((*i).first == name) 
            return (*i).second;
        i++;
    }
    return setup_struct (name);
}

string Corpus::get_info()
{
    string infostr = get_conf ("INFO");
    if (infostr.empty() || infostr[0] != '@')
        return infostr;
    string s (infostr, 1);
    MapBinFile<char> infofile (get_conf ("PATH") + s);
    s = infofile.at(0);
    return s;
}


PosAttr *findPosAttr (CorpInfo *ci, const string &attr_name)
{
    string aname =  attr_name != "-" ? attr_name : ci->opts ["DEFAULTATTR"];

    int dotidx = aname.find ('.');
    if (dotidx >= 0) {
        // struct_name.attr_name
        string struct_name (aname, 0, dotidx);
        aname = string (aname, dotidx +1);
        ci = ci->find_struct (struct_name);
    }

    CorpInfo::MSS ao = ci->find_attr (aname);
    string path_aname = ci->opts ["PATH"] + aname;
    if (ao["DYNAMIC"] != "")
        return createDynAttr (ao, path_aname, aname,
                              findPosAttr (ci, ao["FROMATTR"]), false);
    else if (ci->opts["VIRTUAL"] != "")
        return setup_virtposattr (setup_virtcorp (ci->opts["VIRTUAL"]), 
                                  path_aname, aname, false);
    else
        return createPosAttr (ao["TYPE"], path_aname,
                              aname, ao["LOCALE"], ci->opts["ENCODING"]);
}

PosAttr *findPosAttr (const string &corpus_name, const string &attr_name)
{
    CorpInfo *ci = loadCorpInfo (corpus_name);
    PosAttr *pa = NULL;
    if (ci) {
        pa = findPosAttr (ci, attr_name);
        delete ci;
    }
    return pa;
}

const string Corpus::get_sizes()
{
    string sizefile = get_conf ("PATH") + "/sizes";
    ifstream f(sizefile.c_str(), ifstream::in);
    string sizes((istreambuf_iterator<char>(f)),istreambuf_iterator<char>());
    f.close();
    return sizes;
}

NumOfPos Corpus::compute_docf(FastStream *poss, RangeStream *struc) {
    Position end = 0, beg;
    NumOfPos freq = 0;
    while (!struc->end()) {
        Position pos = poss->find(end);
        if (poss->peek() >= poss->final()) {
            break;
        }
        beg = struc->find_end(pos);
        if (beg <= pos) {
            freq++;
            end = struc->peek_end() + 1;
        } else {
            end = struc->peek_beg();
        }
    }
    return freq;
}
// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

