//  Copyright (c) 2000-2011  Pavel Rychly

#ifndef DYNATTR_HH
#define DYNATTR_HH

#include "dynfun.hh"
#include "posattr.hh"

PosAttr *createDynAttr (const std::string &type, const std::string &apath, 
            const std::string &n,
            DynFun *fun, PosAttr *from, const std::string &locale,
            bool trasquery=false, bool ownedByCorpus = true);

#endif

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:
