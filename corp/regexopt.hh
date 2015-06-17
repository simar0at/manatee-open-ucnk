// Copyright (c) 2014 Milos Jakubicek

#ifndef REGEXOPT_HH
#define REGEXOPT_HH

#include <finlib/frstream.hh>
#include "posattr.hh"

FastStream * optimize_regex (PosAttr *a, const char *pat, const char *encoding);

#endif
