//  Copyright (c) 1999-2013  Pavel Rychly, Milos Jakubicek

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "corpconfLexer.hh"
#include "corpconfParser.hh"
#include "corpconf.hh"
#include "config.hh"

using namespace std;

#ifndef MANATEE_REGISTRY_STR
#define MANATEE_REGISTRY_STR "/corpora/registry"
#endif

CorpInfo *loadCorpInfo (const string &corp_name_or_path)
{

    string config_path;
    ifstream input;
    struct stat dirstat;
    const char *registry = MANATEE_REGISTRY_STR;
    if (getenv ("MANATEE_REGISTRY") != NULL) {
        registry = getenv ("MANATEE_REGISTRY");
    }
    if (corp_name_or_path[0] != '.' && corp_name_or_path[0] != '/' && strlen(registry) > 0) {
        // find first config in ':' separated list of directories
        istringstream rs (registry);
        string regdir;
        while (getline (rs, regdir, ':')) {
            config_path = regdir + "/" + corp_name_or_path;
            if (stat(config_path.c_str(), &dirstat) != -1) {
                if ((dirstat.st_mode & S_IFMT) == S_IFDIR) {
                    continue; // skip directories since they succeed to be "opened" and !fail()
                }
            }
            input.open (config_path.c_str());
            if (!input.fail()) {
                break;
            }
        }
    } else {
        config_path = corp_name_or_path;
        if (stat(config_path.c_str(), &dirstat) != -1) {
            if ((dirstat.st_mode & S_IFMT) != S_IFDIR) {
                input.open (config_path.c_str());
            }
        }
    }
    if (input.fail() || !input.is_open()) {
        throw CorpInfoNotFound (config_path);
    }
    input.close();

#ifdef MANATEE_ANTLR_34_API
    pANTLR3_INPUT_STREAM inputStream = antlr3FileStreamNew((pANTLR3_UINT8) config_path.c_str(), ANTLR3_ENC_8BIT);
#else
    pANTLR3_INPUT_STREAM inputStream = antlr3AsciiFileStreamNew((pANTLR3_UINT8) config_path.c_str());
#endif
    if (inputStream == NULL) {
        throw bad_alloc();
    }
    pcorpconfLexer lexer = corpconfLexerNew(inputStream);
    if (lexer == NULL) {
        throw bad_alloc();
    }
    pANTLR3_COMMON_TOKEN_STREAM tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
    if (tokenStream == NULL) {
        throw bad_alloc();
    }
    pcorpconfParser parser = corpconfParserNew(tokenStream);
    if (parser == NULL) {
        throw bad_alloc();
    }
    CorpInfo *c = new CorpInfo();
    parser->config (parser, *c);
    c->set_defaults();
    c->conffile = config_path;
    
    parser->free(parser);
    tokenStream->free(tokenStream);
    lexer->free(lexer);
    inputStream->close(inputStream);
    inputStream = NULL;
    parser = NULL;
    tokenStream = NULL;
    lexer = NULL;
    
    return c;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

