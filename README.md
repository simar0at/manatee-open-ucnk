Manatee-open-ucnk
=================

This is a fork of *Manatee-open-1.121.1* corpus management and query system.
It represents a slightly customized version as used in production 
by the Czech National Corpus. 

To build the project, [finlib-2.31](http://corpora.fi.muni.cz/noske/src/finlib/finlib-2.31.tar.gz)
must be installed.

This project is intended mainly for use by the Czech National Corpus and 
unless you are looking for our specific modifications you probably want
to look at [NoSketch Engine page](http://nlp.fi.muni.cz/trac/noske) where
you can download original and more recent versions of the software.


List of modifications
---------------------

* *conc/concedit.cc* - Concordance::shuffle() modified to produce replicable results in any case
* *corp/dynfun.cc* - removed code in *DynFun_base::DynFun_base()* referring to dependencies
  not present in the original package (external dynamic attributes).
