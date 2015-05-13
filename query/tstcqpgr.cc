// Copyright (c) 1999-2012 Pavel Rychly

#include <finlib/seek.hh>
#include <cstdio>
#include "cqpLexer.hpp"
#include "cqpParser.hpp"
#include "cqpTreeWalker.hpp"
#include "cqpeval.hh"
#include "config.hh"
#include <iostream>

using namespace std;

int main(int argc, char ** argv)
{
    const char *corpname = "susanne";
    
    if (argc > 1) {
        corpname = argv[1];
    }

    string inputQuery;
    pcqpLexer lexer;
    pcqpParser parser;
    cqpParser_query_return cqpAST;
    pcqpTreeWalker treeWalker;
    pANTLR3_INPUT_STREAM input;
    pANTLR3_COMMON_TOKEN_STREAM tokenStream;
    pANTLR3_COMMON_TREE_NODE_STREAM nodes;
    
    try {
        getline(cin, inputQuery);
#ifdef ANTLR_34_API
        input = antlr3StringStreamNew ((pANTLR3_UINT8) inputQuery.c_str(), ANTLR3_ENC_8BIT,
                (ANTLR3_UINT32) inputQuery.length(),(pANTLR3_UINT8)"CQP Stream");
#else
        input = antlr3NewAsciiStringInPlaceStream ((pANTLR3_UINT8) inputQuery.c_str(),
                (ANTLR3_UINT32) inputQuery.length(),(pANTLR3_UINT8)"CQP Stream");
#endif
        if (input == NULL) {
            fprintf(stderr, 
                    "Unable to set up input stream due to malloc() failure\n");
            exit(ANTLR3_ERR_NOMEM);
        }

        lexer = cqpLexerNew(input);
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

        parser = cqpParserNew(tokenStream);
        if (parser == NULL) {
            fprintf(stderr, "Out of memory trying to allocate parser\n");
            exit(ANTLR3_ERR_NOMEM);
        }

        Corpus *corp = new Corpus (corpname);
        PosAttr *a = corp->get_attr ("word");
        printf ("Corpus %s found.\n", corpname);
        RangeStream *r = NULL;
        FastStream *fs = NULL;

        // Parse the input expression
        cqpAST = parser->query(parser);
        if (!cqpAST.tree) {
            puts ("No tree produced!");
        } else {
            // Print the resulting tree out in LISP notation
            puts ((const char *) cqpAST.tree->toStringTree(cqpAST.tree)->chars);
            nodes = antlr3CommonTreeNodeStreamNewTree(
                    (ANTLR3_BASE_TREE_struct*) cqpAST.tree, ANTLR3_SIZE_HINT);
            if (nodes == NULL) {
                fprintf(stderr, 
                        "Out of memory trying to allocate tree nodes\n");
                exit(ANTLR3_ERR_NOMEM);
            }
            treeWalker = cqpTreeWalkerNew(nodes);
            if (treeWalker == NULL) {
                fprintf(stderr, 
                        "Out of memory trying to allocate tree walker\n");
                exit(ANTLR3_ERR_NOMEM);
            }
            // Traverse the tree created by the parser
            r = treeWalker->query (treeWalker, *corp);
            puts ("Stream gained.");
            if (r == NULL || r->peek_beg() >= r->final()) {
                puts ("Nothing found!");
            } else {
                printf("stream: min=%lli max=%lli\n",r->rest_min(),r->rest_max());
                int maxlines = 5;
                Labels lll;
                int rescount = 0;
                do {
                    rescount++;
                    int from = r->peek_beg();
                    int to = r->peek_end();
                    printf ("%i[%i]: <", from, to - from);
                    for (int i=from; i < to; i++) {
                        printf ("%s ", a->pos2str (i));
                    }
                    printf ("> ");
                    for (int i=to; i < to +5; i++) {
                        printf ("%s ", a->pos2str (i));
                    }
                    lll.clear();
                    r->add_labels (lll);
                    for (Labels::iterator i = lll.begin(); 
                         i!= lll.end(); ++i) {
                        printf ("<<%d:%lli>>", (*i).first, (*i).second);
                    }
                    puts ("");
                    //if (from  > 400) break;
                } while (r->next() && --maxlines);
                if (r->peek_beg() < r->final()) {
                    do {
                        rescount++;
                    } while (r->next()); 
                }
                printf ("rstream: finished %i\n", rescount);
            }
            if (r != NULL) {
                delete r;
            }
            if (fs) {
                printf ("fstream: min=%lli  max=%lli\n", fs->rest_min(),
                        fs->rest_max());
                int l = fs->final();
                int pos;
                while ((pos = fs->next()) < l) {
                    printf ("%i: ", pos);
                    for (int i=pos; i < pos + 5; i++) {
                        printf ("%s ", a->pos2str (i));
                    }
                    puts ("");
                }
                puts ("fstream: finished");
                delete fs;
            }
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
