// Copyright (c) 1999-2010 Pavel Rychly, Milos Jakubicek

#include "cqpeval.hh"
#include "frsop.hh"
#include <finlib/fsop.hh>
#include <iostream>
#include <cstdio>

using namespace std;

int main(int argc, char ** argv)
{
	const char *corpname = "susanne";
	if (argc > 2)
		corpname = argv[2];
	if (argc <= 1) {
		printf ("usage: tstcqpgr2 QUERY [CORPUS]\n");
		return 1;
	}

	/*
	 * Input ***MUST*** be allocated on heap otherwise segfault since we use the InPlace constructor for ANTLR stream
	 */
	char * inputQuery = strdup(argv[1]);

	try {
		Corpus *corp = new Corpus (corpname);
		PosAttr *a = corp->get_attr ("word");
		//puts ("mam corp");
		RangeStream *r = NULL;
		FastStream *fs = NULL;
		
		r = eval_cqpquery (inputQuery, corp);
		/*r = new RQinNode(//new Pos2Range(new SequenceStream(10,12,13)),
			 new whole_range(&(corp->get_struct("head")->rng)),
			 new whole_range(&(corp->get_struct("doc")->rng)));*/
		//Structure *st = corp->get_struct("head"); 
		//ranges::rng_iter rit (st->rng.rngf.at(10));
		//r = new whole_range(rif, rit);
		//r = new whole_range(&st->rng);
		//fs = eval_cqponepos (argv[1], corp);
		puts ("mam stream");
		if (r == NULL || r->peek_beg() >= r->final())
			puts ("Nothing found!");
		else {
			printf ("stream: min=%i  max=%i\n", int(r->rest_min()), int(r->rest_max()));
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
				for (Labels::iterator i = lll.begin(); i!= lll.end(); ++i)
				printf ("<<%d:%d>>", int((*i).first), int((*i).second));
				puts ("");
				//if (from	> 400) break;
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
			printf ("fstream: min=%d  max=%d\n", int(fs->rest_min()), int(fs->rest_max()));
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
		}
	} catch (exception &e) {
		printf ("exception: %s\n", e.what());
	}
	free(inputQuery);
	return 0;
}
