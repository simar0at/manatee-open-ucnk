//  Copyright (c) 2014 Milos Jakubicek

#include <antlr3.h>
#include <finlib/fsop.hh>
#include "posattr.hh"
#include "config.hh"
#include "cqpeval.hh"
#include "regexoptLexer.hh"
#include "regexoptParser.hh"
#include "regexoptTreeWalker.hh"

using namespace std;

FastStream* optimize_regex (PosAttr *attr, const char *pattern, const char *encoding)
{
    if (!strncmp("(?i)", pattern, 4))
        return NULL;
#ifdef MANATEE_ANTLR_34_API
    int enc;
    if (!strcmp(encoding, "UTF-8"))
        enc = ANTLR3_ENC_UTF8;
    else
        enc = ANTLR3_ENC_8BIT;
    pANTLR3_INPUT_STREAM input = antlr3StringStreamNew ((pANTLR3_UINT8) pattern, enc, (ANTLR3_UINT32) strlen(pattern), (pANTLR3_UINT8) "Regex");
#else
    if (!strcmp(encoding, "UTF-8"))
        return NULL;
    pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringInPlaceStream ((pANTLR3_UINT8) pattern, (ANTLR3_UINT32) strlen(pattern), (pANTLR3_UINT8) "Regex");
#endif
    if (input == NULL) {
        throw std::bad_alloc();
    }

    pregexoptLexer lexer = regexoptLexerNew(input);
    if (lexer == NULL) {
        throw std::bad_alloc();
    }
    
    pANTLR3_COMMON_TOKEN_STREAM tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
    if (tokenStream == NULL) {
        throw std::bad_alloc();
    }

    pregexoptParser parser = regexoptParserNew(antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer)));
    if (parser == NULL) {
        throw std::bad_alloc();
    }
    
    FastStream *f = NULL;
    const char *errmsg = NULL;
    
    regexoptParser_regex_return regexoptAST = parser->regex(parser);
    if (parser->pParser->rec->getNumberOfSyntaxErrors(parser->pParser->rec) > 0) {
        throw EvalQueryException ("Internal error: couldn't recover from parser failure.");
    } else {
        pANTLR3_COMMON_TREE_NODE_STREAM nodes = antlr3CommonTreeNodeStreamNewTree((ANTLR3_BASE_TREE_struct*) regexoptAST.tree, ANTLR3_SIZE_HINT);
        if (nodes == NULL) {
            throw std::bad_alloc();
        }
        pregexoptTreeWalker treeWalker = regexoptTreeWalkerNew(nodes);
        if (treeWalker == NULL) {
            throw std::bad_alloc();
        }
        f = treeWalker->regexopt (treeWalker, *attr);
        nodes->free(nodes);
        treeWalker->free(treeWalker);
        nodes = NULL;
        treeWalker = NULL;
    }

    parser->free(parser);
    tokenStream->free(tokenStream);
    lexer->free(lexer);
    input->free(input);
    parser = NULL;
    tokenStream = NULL;
    lexer = NULL;
    input = NULL;

    if (!f) {
        errmsg = "Internal error: couldn't recover from tree walker failure.";
    }
    if (errmsg) {
        throw EvalQueryException (errmsg);
    }

    return f;
}

FastStream* str2fs (PosAttr *a, vector<pair<const char*, bool> > *nodes,
                    int from, int to)
{
    ostringstream oss;
    bool is_regex = false;
    vector<pair<const char*, bool> >::const_iterator it,
                                                     end = nodes->begin() + to;
    for (it = nodes->begin() + from; it != end; it++) {
        if (it->second) {
            is_regex = true;
            break;
        }
    }
    for (it = nodes->begin() + from; it != end; it++) {
        if (!is_regex && it->first[0] == '\\')
            oss << it->first + 1;
        else if (is_regex && (it->first[0] == '^' || it->first[0] == '$'))
            oss << '\\' << it->first;
        else
            oss << it->first;
    }
    string str = oss.str();
// DEBUG: fprintf (stderr, "str2fs: %s, is_regex = %d\n", str.c_str(), is_regex);
    if (!is_regex)
        return a->dynid2srcids (a->str2id (str.c_str()));
    FastStream *fs = new Gen2Fast<int> (a->regexp2ids (str.c_str(), false));
    Position id;
    vector<FastStream*> *fsv = new vector<FastStream*>();
    fsv->reserve (32);
    while ((id = fs->next()) < fs->final())
        fsv->push_back (a->dynid2srcids (id));
    switch (fsv->size()) {
        case 0:
            delete fsv;
            return new EmptyStream();
        case 1: {
            FastStream *f = fsv->front();
            delete fsv;
            return f; }
        default:
            return new QOrVNode (fsv);
    }
}

FastStream *nodes2fs (PosAttr *a, vector< pair<const char*, bool> > *nodes)
{
    size_t len = nodes->size();
    if (len <= 3)
        return str2fs (a, nodes, 0, len);
    FastStream *f = str2fs (a, nodes, 0, 3);
    for (size_t i = 1; i <= len - 3; i++)
        f = new QAndNode (f, str2fs (a, nodes, i, i + 3));
    return f;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
