//  Copyright (c) 1999-2012  Pavel Rychly

#include "config.hh"
#include "corpconf.hh"
#include "langcodes.cc"

CorpInfo::CorpInfo (type_t t)
    :type (t)
{
}

CorpInfo::CorpInfo (CorpInfo *x)
    :type (x->type), conffile (x->conffile)
{
    for (MSS::iterator i = x->opts.begin(); i != x->opts.end(); i++)
        opts [(*i).first] = (*i).second;
    for (VSC::iterator i = x->attrs.begin(); i != x->attrs.end(); i++)
        attrs.push_back (pair<string,CorpInfo*>((*i).first, 
                                                new CorpInfo ((*i).second)));
    for (VSC::iterator i = x->structs.begin(); i != x->structs.end(); i++)
        structs.push_back (pair<string,CorpInfo*>((*i).first, 
                                                  new CorpInfo ((*i).second)));
    for (VSC::iterator i = x->procs.begin(); i != x->procs.end(); i++)
        procs.push_back (pair<string,CorpInfo*>((*i).first, 
                                                new CorpInfo ((*i).second)));
}

CorpInfo::~CorpInfo () {
    for (VSC::iterator i = attrs.begin(); i != attrs.end(); i++)
        delete (*i).second;
    for (VSC::iterator i = structs.begin(); i != structs.end(); i++)
        delete (*i).second;
    for (VSC::iterator i = procs.begin(); i != procs.end(); i++)
        delete (*i).second;
}

string CorpInfo::dump (int indent) {
    string res;
    string space;
    space.resize (indent, ' ');
    for (MSS::iterator i = opts.begin(); i != opts.end(); i++)
        res += space + (*i).first + " = " + (*i).second + "\n";
    for (VSC::iterator i = attrs.begin(); i != attrs.end(); i++)
        res += space + "ATTRIBUTE " + (*i).first + " = {\n" 
            + (*i).second->dump (indent +4) + space + "}\n";
    for (VSC::iterator i = structs.begin(); i != structs.end(); i++)
        res += space + "STRUCTURE " +(*i).first + " = {\n" 
            + (*i).second->dump (indent +4) + space + "}\n";
    for (VSC::iterator i = procs.begin(); i != procs.end(); i++)
        res += space + "PROCESS " +(*i).first + " = {\n" 
            + (*i).second->dump (indent +4) + space + "}\n";
    return res;
}

static void set_defs_from_array (CorpInfo::MSS &opts, 
                                 const char * const *opt_defs)
{
    while (*opt_defs) {
        if (opts [opt_defs[0]] == "")
            opts [opt_defs[0]] = opt_defs[1];
        opt_defs +=2;
    }
}

void CorpInfo::set_defaults (CorpInfo *global, type_t d_type) {
    static const char *attr_defs[] 
        = {"ENCODING", "", "MULTIVALUE", "n", "MULTISEP", ",", 
           "TYPE", "default", "DYNTYPE", "index",
           NULL};
    static const char *struct_attr_defs[] = {"TYPE", "MD_MI", NULL};

    const char **opt_defs = NULL;
    if (type == Unset_type)
        type = d_type;
    switch (type) {
    case Corpus_type:
        if (opts["DEFAULTATTR"].empty()) {
            if (attrs.size() > 0)
                opts["DEFAULTATTR"] = attrs[0].first;
            else
                opts["DEFAULTATTR"] = "word";
        }
        if (!opts["PATH"].empty()) {
            string path = opts ["PATH"];
            if (*(path.end() -1) != '/') {
                path += '/';
                opts ["PATH"] = path;
            }
        }
        if ((opts["ENCODING"] == "utf-8") || 
            !strcasecmp(opts["ENCODING"].c_str(), "utf8")) {
            opts["ENCODING"] = "UTF-8";
        }
        if (opts["DOCSTRUCTURE"] == "") {
            opts["DOCSTRUCTURE"] = "doc";
        }
        if (opts["NONWORDRE"] == "") {
            opts["NONWORDRE"] = "[^[:alpha:]].*";
        }
        if (opts["MAXCONTEXT"] == "") {
            opts["MAXCONTEXT"] = "100";
        }
        if (opts["MAXDETAIL"] == "") {
            opts["MAXDETAIL"] = opts["MAXCONTEXT"];
        }
        if (opts["ALIGNSTRUCT"] == "") {
            opts["ALIGNSTRUCT"] = "align";
        }
        for (VSC::iterator i = attrs.begin(); i != attrs.end(); i++)
            (*i).second->set_defaults (this, Attr_type);
        for (VSC::iterator i = structs.begin(); i != structs.end(); i++) {
            (*i).second->opts["PATH"] = opts ["PATH"] + (*i).first + '.';
            (*i).second->set_defaults (this, Struct_type);
        }
        for (VSC::iterator i = procs.begin(); i != procs.end(); i++)
            (*i).second->set_defaults (this, Proc_type);
        break;
    case Attr_type:
        opt_defs = attr_defs;
        if (opts["LOCALE"].empty()) {
            if (!global->opts["DEFAULTLOCALE"].empty()) {
                opts["LOCALE"] = global->opts["DEFAULTLOCALE"];
                break;
            }
            if (!global->opts["LANGUAGE"].empty()) {
                const char* langcode =
                    (*langcodes) [global->opts["LANGUAGE"].c_str()];
                if (langcode)
                    try {
                        opts["LOCALE"] = string(langcode) + "." +
                                         global->opts["ENCODING"];
                        locale(opts["LOCALE"].c_str()); // test availability
                        global->opts["DEFAULTLOCALE"] = opts["LOCALE"];
                        break;
                    } catch (...) {}
            }
            opts["LOCALE"] = global->opts["DEFAULTLOCALE"] = "C";
        }
        if (str2bool (global->opts["NESTED"]))
            opts["MULTIVALUE"] = "y";
        break;
    case Struct_type:
        for (VSC::iterator i = attrs.begin(); i != attrs.end(); i++) {
            set_defs_from_array ((*i).second->opts, struct_attr_defs);
            (*i).second->set_defaults (this, Attr_type);
        }
        break;
    case Proc_type:
        /* nothing to do */
        break;
    case Unset_type:
        /* dead code */ break;
    }
    if (opt_defs)
        set_defs_from_array (opts, opt_defs);
}

CorpInfo* CorpInfo::find_sub (const string &val, VSC &v)
{
    for (VSC::iterator i = v.begin(); i != v.end(); i++) {
        if ((*i).first == val)
            return (*i).second;
    }
    throw CorpInfoNotFound (val);
}

CorpInfo::MSS& CorpInfo::find_attr (const string &attr)
{
    int dotidx = attr.find ('.');
    if (dotidx >= 0) {
        // struct_name.attr_name
        string struct_name (attr, 0, dotidx);
        string attr_name (attr, dotidx +1);
        return find_sub (attr_name, 
                         find_sub (struct_name, structs)->attrs)->opts;
    } else 
        return find_sub (attr, attrs)->opts;
}

string &CorpInfo::find_opt (const string &path)
{
    int i;
    if ((i = path.find ('.')) >= 0) {
        string first (path, 0, i);
        string rest (path, i+1);
        try {
            return find_attr (first)[rest];
        } catch (CorpInfoNotFound &e) {
            try {
                return find_sub (first, procs)->opts [rest];
            } catch (CorpInfoNotFound &e) {
                return find_struct (first)->find_opt (rest);
            }
        }
    } else {
        string val = opts[path];
        if (val.empty()) {
            if (path == "FULLREF") {
                val = find_opt ("STRUCTATTRLIST");
                if (val.empty())
                    val = "#";
            } else if (path == "SHORTREF") {
                string doc = find_opt ("DOCSTRUCTURE");
                try {
                    find_attr(doc + ".id");
                    val = '=' + doc + ".id";
                } catch (CorpInfoNotFound) {
                    try {
                        find_struct(doc);
                        val = doc;
                    } catch (CorpInfoNotFound) {
                        string refs = find_opt ("STRUCTATTRLIST");
                        if ((i = refs.find (',')) >= 0)
                            refs.erase (i);
                        if (refs.empty())
                            val = "#";
                        else
                            val = '=' + refs;
                    }
                }
            } else if (path == "ATTRLIST") {
                for (VSC::iterator j = attrs.begin(); j != attrs.end(); j++)
                    val += (*j).first + ',';
            } else if (path == "STRUCTLIST") {
                for (VSC::iterator j=structs.begin(); j != structs.end(); j++)
                    val += (*j).first + ',';
            } else if (path == "STRUCTATTRLIST") {
                for (VSC::iterator j=structs.begin(); j != structs.end(); j++)
                    for (VSC::iterator k = (*j).second->attrs.begin(); 
                         k != (*j).second->attrs.end(); k++)
                        val += (*j).first + '.' + (*k).first + ',';
            } else if (path == "PROCLIST") {
                for (VSC::iterator j=procs.begin(); j != procs.end(); j++)
                    val += (*j).first + ',';
            } 
#ifdef SKETCH_ENGINE
            else if (path == "WSBASE") {
                return opts[path] = opts["PATH"] + find_opt("WSATTR") + "-ws";
            } else if (path == "WSTHES") {
                return opts[path] = opts["PATH"] + find_opt("WSATTR") + "-thes";
            } else if (path == "WSATTR") {
                try {
                    find_sub ("lempos", attrs);
                    return opts[path] = "lempos";
                } catch (CorpInfoNotFound &e){}
                try {
                    find_sub ("lemma", attrs);
                    return opts[path] = "lemma";
                } catch (CorpInfoNotFound &e){}
                return opts["DEFAULTATTR"];
            } else if (path == "WSSTRIP") {
                if (find_opt ("WSATTR") == "lempos")
                    return opts[path] = "2";
                else
                    return opts[path] = "0";
            }
#endif
            if (!val.empty() && *(val.end() -1) == ',')
                val.erase (val.length() -1);
        }
        return (opts[path] = val);
    }
}


bool CorpInfo::str2bool (const string &str)
{
    if (str == "y" || str == "yes" || str == "t" || str == "true" || 
        str == "1")
        return true;
    if (str == "n" || str == "no" || str == "f" || str == "false" || 
        str == "0")
        return false;
    //throw xxx;
    return false;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

