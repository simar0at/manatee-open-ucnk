// Copyright (c) 2014 Pavel Rychly, Milos Jakubicek

#include <antlr3.h>
#include <finlib/seek.hh>
#include <cstdio>
#include "regexoptLexer.hh"
#include "regexoptParser.hh"
#include "regexoptTreeWalker.hh"
//#include "cqpeval.hh"
#include "config.hh"
#include "dynattr.hh"
#include <iostream>

using namespace std;

int main(int argc, char ** argv)
{
    const char *corpname = "susanne";
    
    if (argc > 1) {
        corpname = argv[1];
    }

    string inputQuery;
    pregexoptLexer lexer;
    pregexoptParser parser;
    regexoptParser_regex_return regexoptAST;
    pregexoptTreeWalker treeWalker;
    pANTLR3_INPUT_STREAM input;
    pANTLR3_COMMON_TOKEN_STREAM tokenStream;
    pANTLR3_COMMON_TREE_NODE_STREAM nodes;
    
    try {
        getline(cin, inputQuery);

#ifdef MANATEE_ANTLR_34_API
        input = antlr3StringStreamNew ((pANTLR3_UINT8) inputQuery.c_str(), ANTLR3_ENC_UTF8,
                (ANTLR3_UINT32) inputQuery.length(),(pANTLR3_UINT8)"Regex");
#else
        input = antlr3NewAsciiStringInPlaceStream ((pANTLR3_UINT8) inputQuery.c_str(),
                (ANTLR3_UINT32) inputQuery.length(),(pANTLR3_UINT8)"Regex");
#endif
        if (input == NULL) {
            fprintf(stderr, 
                    "Unable to set up input stream due to malloc() failure\n");
            exit(ANTLR3_ERR_NOMEM);
        }

        lexer = regexoptLexerNew(input);
        if (lexer == NULL) {
            fprintf(stderr, 
                    "Unable to create the lexer due to malloc() failure\n");
            exit(ANTLR3_ERR_NOMEM);
        }

        tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, 
                                                       TOKENSOURCE(lexer));
        if (tokenStream == NULL) {
            fprintf(stderr, "Out of memory trying to allocate token stream\n");
            exit(ANTLR3_ERR_NOMEM);
        }

        pANTLR3_VECTOR tokens = tokenStream->getTokens (tokenStream);
        for (ANTLR3_UINT32 i = 0; i < tokens->size(tokens); i++) {
            ANTLR3_COMMON_TOKEN_struct* token = (ANTLR3_COMMON_TOKEN_struct*)tokens->get(tokens, i);
            puts ((const char *) token->toString(token)->chars);
        }

        parser = regexoptParserNew(tokenStream);
        if (parser == NULL) {
            fprintf(stderr, "Out of memory trying to allocate parser\n");
            exit(ANTLR3_ERR_NOMEM);
        }

        Corpus *corp = new Corpus (corpname);
        PosAttr *src = corp->get_attr ("word");
        DynFun *fun = createDynFun ("", "internal", "lowercase"); // lowercase = dummy
        PosAttr *a = createDynAttr ("index", src->attr_path + ".regex",
                                    src->name + ".regex", fun, src, src->locale,
                                    false);
        printf ("Corpus %s found.\n", corpname);
        FastStream *f = NULL;

        // Parse the input expression
        regexoptAST = parser->regex(parser);
        if (!regexoptAST.tree) {
            puts ("No tree produced!");
        } else {
            // Print the resulting tree out in LISP notation
            puts ((const char *) regexoptAST.tree->toStringTree(regexoptAST.tree)->chars);
            nodes = antlr3CommonTreeNodeStreamNewTree(
                    (ANTLR3_BASE_TREE_struct*) regexoptAST.tree, ANTLR3_SIZE_HINT);
            if (nodes == NULL) {
                fprintf(stderr, 
                        "Out of memory trying to allocate tree nodes\n");
                exit(ANTLR3_ERR_NOMEM);
            }
            treeWalker = regexoptTreeWalkerNew(nodes);
            if (treeWalker == NULL) {
                fprintf(stderr, 
                        "Out of memory trying to allocate tree walker\n");
                exit(ANTLR3_ERR_NOMEM);
            }
            // Traverse the tree created by the parser
            f = treeWalker->regexopt (treeWalker, *a);
            puts ("Stream gained.");
            if (f == NULL)
                puts ("Stream is null");
            else if (f->peek() >= f->final()) {
                puts ("Nothing found!");
            } else {
                printf("stream: min=%lli max=%lli\n",f->rest_min(),f->rest_max());
                int maxlines = 45;
                Labels lll;
                int rescount = 0;
                int id;
                while ((id = f->next()) < f->final() && maxlines--) {
                    rescount++;
                    printf ("%s\n", src->id2str (id));
                }
                while (f->next() < f->final())
                    rescount++;
                printf ("stream: finished %i\n", rescount);
            }
            delete f;
            nodes->free(nodes);
            nodes = NULL;
            treeWalker->free(treeWalker);
            treeWalker = NULL;
        }
        
        parser->free(parser);
        parser = NULL;
        tokenStream->free(tokenStream);
        tokenStream = NULL;
        lexer->free(lexer);
        lexer = NULL;

    } catch (exception &e) {
        printf ("exception: %s\n", e.what());
    }
    
    return 0;
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
