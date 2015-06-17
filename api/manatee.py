# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.11
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_manatee', [dirname(__file__)])
        except ImportError:
            import _manatee
            return _manatee
        if fp is not None:
            try:
                _mod = imp.load_module('_manatee', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _manatee = swig_import_helper()
    del swig_import_helper
else:
    import _manatee
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class manatee_SwigPyIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, manatee_SwigPyIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, manatee_SwigPyIterator, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _manatee.delete_manatee_SwigPyIterator
    __del__ = lambda self : None;
    def value(self): return _manatee.manatee_SwigPyIterator_value(self)
    def incr(self, n=1): return _manatee.manatee_SwigPyIterator_incr(self, n)
    def decr(self, n=1): return _manatee.manatee_SwigPyIterator_decr(self, n)
    def distance(self, *args): return _manatee.manatee_SwigPyIterator_distance(self, *args)
    def equal(self, *args): return _manatee.manatee_SwigPyIterator_equal(self, *args)
    def copy(self): return _manatee.manatee_SwigPyIterator_copy(self)
    def __next__(self): return _manatee.manatee_SwigPyIterator___next__(self)
    def previous(self): return _manatee.manatee_SwigPyIterator_previous(self)
    def advance(self, *args): return _manatee.manatee_SwigPyIterator_advance(self, *args)
    def __eq__(self, *args): return _manatee.manatee_SwigPyIterator___eq__(self, *args)
    def __ne__(self, *args): return _manatee.manatee_SwigPyIterator___ne__(self, *args)
    def __iadd__(self, *args): return _manatee.manatee_SwigPyIterator___iadd__(self, *args)
    def __isub__(self, *args): return _manatee.manatee_SwigPyIterator___isub__(self, *args)
    def __add__(self, *args): return _manatee.manatee_SwigPyIterator___add__(self, *args)
    def __sub__(self, *args): return _manatee.manatee_SwigPyIterator___sub__(self, *args)
    def __iter__(self): return self
    def next(self, *args):
        val = _manatee.manatee_SwigPyIterator_next(self, *args)
        if getEncoding() and type(val) == str:
            val = unicode(val, getEncoding(), errors='replace')


        return val

manatee_SwigPyIterator_swigregister = _manatee.manatee_SwigPyIterator_swigregister
manatee_SwigPyIterator_swigregister(manatee_SwigPyIterator)

class FileAccessError(Exception):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, FileAccessError, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, FileAccessError, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _manatee.new_FileAccessError(*args)
        try: self.this.append(this)
        except: self.this = this
    def __str__(self): return _manatee.FileAccessError___str__(self)
    __swig_destroy__ = _manatee.delete_FileAccessError
    __del__ = lambda self : None;
FileAccessError_swigregister = _manatee.FileAccessError_swigregister
FileAccessError_swigregister(FileAccessError)

class AttrNotFound(Exception):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, AttrNotFound, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, AttrNotFound, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _manatee.new_AttrNotFound(*args)
        try: self.this.append(this)
        except: self.this = this
    def __str__(self): return _manatee.AttrNotFound___str__(self)
    __swig_destroy__ = _manatee.delete_AttrNotFound
    __del__ = lambda self : None;
AttrNotFound_swigregister = _manatee.AttrNotFound_swigregister
AttrNotFound_swigregister(AttrNotFound)

class ConcNotFound(Exception):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ConcNotFound, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ConcNotFound, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _manatee.new_ConcNotFound(*args)
        try: self.this.append(this)
        except: self.this = this
    def __str__(self): return _manatee.ConcNotFound___str__(self)
    __swig_destroy__ = _manatee.delete_ConcNotFound
    __del__ = lambda self : None;
ConcNotFound_swigregister = _manatee.ConcNotFound_swigregister
ConcNotFound_swigregister(ConcNotFound)

class CorpInfoNotFound(Exception):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CorpInfoNotFound, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CorpInfoNotFound, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _manatee.new_CorpInfoNotFound(*args)
        try: self.this.append(this)
        except: self.this = this
    def __str__(self): return _manatee.CorpInfoNotFound___str__(self)
    __swig_destroy__ = _manatee.delete_CorpInfoNotFound
    __del__ = lambda self : None;
CorpInfoNotFound_swigregister = _manatee.CorpInfoNotFound_swigregister
CorpInfoNotFound_swigregister(CorpInfoNotFound)

class FastStream(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, FastStream, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, FastStream, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def next(self): return _manatee.FastStream_next(self)
    def peek(self): return _manatee.FastStream_peek(self)
    def find(self, *args): return _manatee.FastStream_find(self, *args)
    def end(self): return _manatee.FastStream_end(self)
    __swig_destroy__ = _manatee.delete_FastStream
    __del__ = lambda self : None;
FastStream_swigregister = _manatee.FastStream_swigregister
FastStream_swigregister(FastStream)

class IntVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, IntVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, IntVector, name)
    __repr__ = _swig_repr
    def iterator(self): return _manatee.IntVector_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _manatee.IntVector___nonzero__(self)
    def __bool__(self): return _manatee.IntVector___bool__(self)
    def __len__(self): return _manatee.IntVector___len__(self)
    def pop(self): return _manatee.IntVector_pop(self)
    def __getslice__(self, *args): return _manatee.IntVector___getslice__(self, *args)
    def __setslice__(self, *args): return _manatee.IntVector___setslice__(self, *args)
    def __delslice__(self, *args): return _manatee.IntVector___delslice__(self, *args)
    def __delitem__(self, *args): return _manatee.IntVector___delitem__(self, *args)
    def __getitem__(self, *args): return _manatee.IntVector___getitem__(self, *args)
    def __setitem__(self, *args): return _manatee.IntVector___setitem__(self, *args)
    def append(self, *args): return _manatee.IntVector_append(self, *args)
    def empty(self): return _manatee.IntVector_empty(self)
    def size(self): return _manatee.IntVector_size(self)
    def clear(self): return _manatee.IntVector_clear(self)
    def swap(self, *args): return _manatee.IntVector_swap(self, *args)
    def get_allocator(self): return _manatee.IntVector_get_allocator(self)
    def begin(self): return _manatee.IntVector_begin(self)
    def end(self): return _manatee.IntVector_end(self)
    def rbegin(self): return _manatee.IntVector_rbegin(self)
    def rend(self): return _manatee.IntVector_rend(self)
    def pop_back(self): return _manatee.IntVector_pop_back(self)
    def erase(self, *args): return _manatee.IntVector_erase(self, *args)
    def __init__(self, *args): 
        this = _manatee.new_IntVector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _manatee.IntVector_push_back(self, *args)
    def front(self): return _manatee.IntVector_front(self)
    def back(self): return _manatee.IntVector_back(self)
    def assign(self, *args): return _manatee.IntVector_assign(self, *args)
    def resize(self, *args): return _manatee.IntVector_resize(self, *args)
    def insert(self, *args): return _manatee.IntVector_insert(self, *args)
    def reserve(self, *args): return _manatee.IntVector_reserve(self, *args)
    def capacity(self): return _manatee.IntVector_capacity(self)
    __swig_destroy__ = _manatee.delete_IntVector
    __del__ = lambda self : None;
IntVector_swigregister = _manatee.IntVector_swigregister
IntVector_swigregister(IntVector)

class NumVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, NumVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, NumVector, name)
    __repr__ = _swig_repr
    def iterator(self): return _manatee.NumVector_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _manatee.NumVector___nonzero__(self)
    def __bool__(self): return _manatee.NumVector___bool__(self)
    def __len__(self): return _manatee.NumVector___len__(self)
    def pop(self): return _manatee.NumVector_pop(self)
    def __getslice__(self, *args): return _manatee.NumVector___getslice__(self, *args)
    def __setslice__(self, *args): return _manatee.NumVector___setslice__(self, *args)
    def __delslice__(self, *args): return _manatee.NumVector___delslice__(self, *args)
    def __delitem__(self, *args): return _manatee.NumVector___delitem__(self, *args)
    def __getitem__(self, *args): return _manatee.NumVector___getitem__(self, *args)
    def __setitem__(self, *args): return _manatee.NumVector___setitem__(self, *args)
    def append(self, *args): return _manatee.NumVector_append(self, *args)
    def empty(self): return _manatee.NumVector_empty(self)
    def size(self): return _manatee.NumVector_size(self)
    def clear(self): return _manatee.NumVector_clear(self)
    def swap(self, *args): return _manatee.NumVector_swap(self, *args)
    def get_allocator(self): return _manatee.NumVector_get_allocator(self)
    def begin(self): return _manatee.NumVector_begin(self)
    def end(self): return _manatee.NumVector_end(self)
    def rbegin(self): return _manatee.NumVector_rbegin(self)
    def rend(self): return _manatee.NumVector_rend(self)
    def pop_back(self): return _manatee.NumVector_pop_back(self)
    def erase(self, *args): return _manatee.NumVector_erase(self, *args)
    def __init__(self, *args): 
        this = _manatee.new_NumVector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _manatee.NumVector_push_back(self, *args)
    def front(self): return _manatee.NumVector_front(self)
    def back(self): return _manatee.NumVector_back(self)
    def assign(self, *args): return _manatee.NumVector_assign(self, *args)
    def resize(self, *args): return _manatee.NumVector_resize(self, *args)
    def insert(self, *args): return _manatee.NumVector_insert(self, *args)
    def reserve(self, *args): return _manatee.NumVector_reserve(self, *args)
    def capacity(self): return _manatee.NumVector_capacity(self)
    __swig_destroy__ = _manatee.delete_NumVector
    __del__ = lambda self : None;
NumVector_swigregister = _manatee.NumVector_swigregister
NumVector_swigregister(NumVector)

class StrVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, StrVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, StrVector, name)
    __repr__ = _swig_repr
    def iterator(self): return _manatee.StrVector_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _manatee.StrVector___nonzero__(self)
    def __bool__(self): return _manatee.StrVector___bool__(self)
    def __len__(self): return _manatee.StrVector___len__(self)
    def pop(self): return _manatee.StrVector_pop(self)
    def __getslice__(self, *args): return _manatee.StrVector___getslice__(self, *args)
    def __setslice__(self, *args): return _manatee.StrVector___setslice__(self, *args)
    def __delslice__(self, *args): return _manatee.StrVector___delslice__(self, *args)
    def __delitem__(self, *args): return _manatee.StrVector___delitem__(self, *args)
    def __getitem__(self, *args):
        val = _manatee.StrVector___getitem__(self, *args)
        if getEncoding():
            val = unicode(val, getEncoding(), errors='replace')


        return val

    def __setitem__(self, *args): return _manatee.StrVector___setitem__(self, *args)
    def append(self, *args): return _manatee.StrVector_append(self, *args)
    def empty(self): return _manatee.StrVector_empty(self)
    def size(self): return _manatee.StrVector_size(self)
    def clear(self): return _manatee.StrVector_clear(self)
    def swap(self, *args): return _manatee.StrVector_swap(self, *args)
    def get_allocator(self): return _manatee.StrVector_get_allocator(self)
    def begin(self): return _manatee.StrVector_begin(self)
    def end(self): return _manatee.StrVector_end(self)
    def rbegin(self): return _manatee.StrVector_rbegin(self)
    def rend(self): return _manatee.StrVector_rend(self)
    def pop_back(self): return _manatee.StrVector_pop_back(self)
    def erase(self, *args): return _manatee.StrVector_erase(self, *args)
    def __init__(self, *args): 
        this = _manatee.new_StrVector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _manatee.StrVector_push_back(self, *args)
    def front(self): return _manatee.StrVector_front(self)
    def back(self): return _manatee.StrVector_back(self)
    def assign(self, *args): return _manatee.StrVector_assign(self, *args)
    def resize(self, *args): return _manatee.StrVector_resize(self, *args)
    def insert(self, *args): return _manatee.StrVector_insert(self, *args)
    def reserve(self, *args): return _manatee.StrVector_reserve(self, *args)
    def capacity(self): return _manatee.StrVector_capacity(self)
    __swig_destroy__ = _manatee.delete_StrVector
    __del__ = lambda self : None;
StrVector_swigregister = _manatee.StrVector_swigregister
StrVector_swigregister(StrVector)


def setEncoding(*args):
  return _manatee.setEncoding(*args)
setEncoding = _manatee.setEncoding

def getEncoding():
  return _manatee.getEncoding()
getEncoding = _manatee.getEncoding

def version():
  return _manatee.version()
version = _manatee.version

def languages(*args):
  return _manatee.languages(*args)
languages = _manatee.languages
class IntGenerator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, IntGenerator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, IntGenerator, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def next(self): return _manatee.IntGenerator_next(self)
    def end(self): return _manatee.IntGenerator_end(self)
    __swig_destroy__ = _manatee.delete_IntGenerator
    __del__ = lambda self : None;
IntGenerator_swigregister = _manatee.IntGenerator_swigregister
IntGenerator_swigregister(IntGenerator)

class RangeStream(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, RangeStream, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, RangeStream, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def next(self): return _manatee.RangeStream_next(self)
    def end(self): return _manatee.RangeStream_end(self)
    def peek_beg(self): return _manatee.RangeStream_peek_beg(self)
    def peek_end(self): return _manatee.RangeStream_peek_end(self)
    def find_beg(self, *args): return _manatee.RangeStream_find_beg(self, *args)
    def find_end(self, *args): return _manatee.RangeStream_find_end(self, *args)
    def count_rest(self): return _manatee.RangeStream_count_rest(self)
    def collocs(self, *args): return _manatee.RangeStream_collocs(self, *args)
    __swig_destroy__ = _manatee.delete_RangeStream
    __del__ = lambda self : None;
RangeStream_swigregister = _manatee.RangeStream_swigregister
RangeStream_swigregister(RangeStream)

class IDIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, IDIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, IDIterator, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def next(self): return _manatee.IDIterator_next(self)
    def __getitem__(self, *args): return _manatee.IDIterator___getitem__(self, *args)
    def __len__(self): return _manatee.IDIterator___len__(self)
    __swig_destroy__ = _manatee.delete_IDIterator
    __del__ = lambda self : None;
IDIterator_swigregister = _manatee.IDIterator_swigregister
IDIterator_swigregister(IDIterator)

class TextIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, TextIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, TextIterator, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def next(self): return _manatee.TextIterator_next(self)
    __swig_destroy__ = _manatee.delete_TextIterator
    __del__ = lambda self : None;
TextIterator_swigregister = _manatee.TextIterator_swigregister
TextIterator_swigregister(TextIterator)

class PosAttr(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, PosAttr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, PosAttr, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def id_range(self): return _manatee.PosAttr_id_range(self)
    def id2str(self, *args): return _manatee.PosAttr_id2str(self, *args)
    def str2id(self, *args): return _manatee.PosAttr_str2id(self, *args)
    def pos2id(self, *args): return _manatee.PosAttr_pos2id(self, *args)
    def pos2str(self, *args): return _manatee.PosAttr_pos2str(self, *args)
    def posat(self, *args): return _manatee.PosAttr_posat(self, *args)
    def textat(self, *args): return _manatee.PosAttr_textat(self, *args)
    def id2poss(self, *args): return _manatee.PosAttr_id2poss(self, *args)
    def dynid2srcids(self, *args): return _manatee.PosAttr_dynid2srcids(self, *args)
    def regexp2poss(self, *args): return _manatee.PosAttr_regexp2poss(self, *args)
    def regexp2ids(self, *args): return _manatee.PosAttr_regexp2ids(self, *args)
    def freq(self, *args): return _manatee.PosAttr_freq(self, *args)
    def docf(self, *args): return _manatee.PosAttr_docf(self, *args)
    def arf(self, *args): return _manatee.PosAttr_arf(self, *args)
    def aldf(self, *args): return _manatee.PosAttr_aldf(self, *args)
    def norm(self, *args): return _manatee.PosAttr_norm(self, *args)
    def size(self): return _manatee.PosAttr_size(self)
    __swig_getmethods__["name"] = _manatee.PosAttr_name_get
    if _newclass:name = _swig_property(_manatee.PosAttr_name_get)
    __swig_destroy__ = _manatee.delete_PosAttr
    __del__ = lambda self : None;
PosAttr_swigregister = _manatee.PosAttr_swigregister
PosAttr_swigregister(PosAttr)


def findPosAttr(*args):
  return _manatee.findPosAttr(*args)
findPosAttr = _manatee.findPosAttr
class Structure(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Structure, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Structure, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def size(self): return _manatee.Structure_size(self)
    def get_attr(self, *args): return _manatee.Structure_get_attr(self, *args)
    __swig_getmethods__["name"] = _manatee.Structure_name_get
    if _newclass:name = _swig_property(_manatee.Structure_name_get)
    def num_at_pos(self, *args): return _manatee.Structure_num_at_pos(self, *args)
    def num_next_pos(self, *args): return _manatee.Structure_num_next_pos(self, *args)
    def beg(self, *args): return _manatee.Structure_beg(self, *args)
    def end(self, *args): return _manatee.Structure_end(self, *args)
    def whole(self): return _manatee.Structure_whole(self)
    def attr_val(self, *args): return _manatee.Structure_attr_val(self, *args)
    __swig_destroy__ = _manatee.delete_Structure
    __del__ = lambda self : None;
Structure_swigregister = _manatee.Structure_swigregister
Structure_swigregister(Structure)

class Corpus(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Corpus, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Corpus, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _manatee.new_Corpus(*args)
        try: self.this.append(this)
        except: self.this = this
    def size(self): return _manatee.Corpus_size(self)
    def search_size(self): return _manatee.Corpus_search_size(self)
    def get_attr(self, *args): return _manatee.Corpus_get_attr(self, *args)
    def get_struct(self, *args): return _manatee.Corpus_get_struct(self, *args)
    def get_info(self): return _manatee.Corpus_get_info(self)
    def get_conf(self, *args): return _manatee.Corpus_get_conf(self, *args)
    def get_conffile(self): return _manatee.Corpus_get_conffile(self)
    def get_confpath(self): return _manatee.Corpus_get_confpath(self)
    def set_default_attr(self, *args): return _manatee.Corpus_set_default_attr(self, *args)
    def filter_query(self, *args): return _manatee.Corpus_filter_query(self, *args)
    def get_sizes(self): return _manatee.Corpus_get_sizes(self)
    def compile_frq(self, *args): return _manatee.Corpus_compile_frq(self, *args)
    def compile_arf(self, *args): return _manatee.Corpus_compile_arf(self, *args)
    def compile_aldf(self, *args): return _manatee.Corpus_compile_aldf(self, *args)
    def compile_docf(self, *args): return _manatee.Corpus_compile_docf(self, *args)
    def freq_dist(self, *args): return _manatee.Corpus_freq_dist(self, *args)
    def Range2Pos(self, *args): return _manatee.Corpus_Range2Pos(self, *args)
    def eval_query(self, *args): return _manatee.Corpus_eval_query(self, *args)
    def filter_fstream(self, *args): return _manatee.Corpus_filter_fstream(self, *args)
    def count_rest(self, *args): return _manatee.Corpus_count_rest(self, *args)
    __swig_destroy__ = _manatee.delete_Corpus
    __del__ = lambda self : None;
Corpus_swigregister = _manatee.Corpus_swigregister
Corpus_swigregister(Corpus)

class Concordance(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Concordance, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Concordance, name)
    __repr__ = _swig_repr
    def save(self, *args): return _manatee.Concordance_save(self, *args)
    def size(self): return _manatee.Concordance_size(self)
    def numofcolls(self): return _manatee.Concordance_numofcolls(self)
    def viewsize(self): return _manatee.Concordance_viewsize(self)
    def fullsize(self): return _manatee.Concordance_fullsize(self)
    def finished(self): return _manatee.Concordance_finished(self)
    def sync(self): return _manatee.Concordance_sync(self)
    def set_collocation(self, *args): return _manatee.Concordance_set_collocation(self, *args)
    def sort(self, *args): return _manatee.Concordance_sort(self, *args)
    def relfreq_sort(self, *args): return _manatee.Concordance_relfreq_sort(self, *args)
    def set_sorted_view(self, *args): return _manatee.Concordance_set_sorted_view(self, *args)
    def reduce_lines(self, *args): return _manatee.Concordance_reduce_lines(self, *args)
    def delete_pnfilter(self, *args): return _manatee.Concordance_delete_pnfilter(self, *args)
    def swap_kwic_coll(self, *args): return _manatee.Concordance_swap_kwic_coll(self, *args)
    def extend_kwic_coll(self, *args): return _manatee.Concordance_extend_kwic_coll(self, *args)
    def sort_idx(self, *args): return _manatee.Concordance_sort_idx(self, *args)
    def distribution(self, *args): return _manatee.Concordance_distribution(self, *args)
    def compute_ARF(self): return _manatee.Concordance_compute_ARF(self)
    def set_linegroup(self, *args): return _manatee.Concordance_set_linegroup(self, *args)
    def set_linegroup_globally(self, *args): return _manatee.Concordance_set_linegroup_globally(self, *args)
    def set_linegroup_at_pos(self, *args): return _manatee.Concordance_set_linegroup_at_pos(self, *args)
    def set_linegroup_from_conc(self, *args): return _manatee.Concordance_set_linegroup_from_conc(self, *args)
    def get_new_linegroup_id(self): return _manatee.Concordance_get_new_linegroup_id(self)
    def delete_linegroups(self, *args): return _manatee.Concordance_delete_linegroups(self, *args)
    def begs_FS(self): return _manatee.Concordance_begs_FS(self)
    def RS(self, useview=False, beg=0, end=0): return _manatee.Concordance_RS(self, useview, beg, end)
    def shuffle(self): return _manatee.Concordance_shuffle(self)
    def beg_at(self, *args): return _manatee.Concordance_beg_at(self, *args)
    def end_at(self, *args): return _manatee.Concordance_end_at(self, *args)
    def coll_beg_at(self, *args): return _manatee.Concordance_coll_beg_at(self, *args)
    def coll_end_at(self, *args): return _manatee.Concordance_coll_end_at(self, *args)
    def switch_aligned(self, *args): return _manatee.Concordance_switch_aligned(self, *args)
    def get_aligned(self, *args): return _manatee.Concordance_get_aligned(self, *args)
    def add_aligned(self, *args): return _manatee.Concordance_add_aligned(self, *args)
    def delete_subparts(self): return _manatee.Concordance_delete_subparts(self)
    def delete_struct_repeats(self, *args): return _manatee.Concordance_delete_struct_repeats(self, *args)
    def __init__(self, *args): 
        this = _manatee.new_Concordance(*args)
        try: self.this.append(this)
        except: self.this = this
    def get_linegroup_stat(self, *args): return _manatee.Concordance_get_linegroup_stat(self, *args)
    def linegroup_sort(self, *args): return _manatee.Concordance_linegroup_sort(self, *args)
    def corp(self): return _manatee.Concordance_corp(self)
    __swig_destroy__ = _manatee.delete_Concordance
    __del__ = lambda self : None;
Concordance_swigregister = _manatee.Concordance_swigregister
Concordance_swigregister(Concordance)

class KWICLines(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, KWICLines, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, KWICLines, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _manatee.new_KWICLines(*args)
        try: self.this.append(this)
        except: self.this = this
    def nextcontext(self): return _manatee.KWICLines_nextcontext(self)
    def nextline(self): return _manatee.KWICLines_nextline(self)
    def skip(self, *args): return _manatee.KWICLines_skip(self, *args)
    def get_pos(self): return _manatee.KWICLines_get_pos(self)
    def get_kwiclen(self): return _manatee.KWICLines_get_kwiclen(self)
    def get_ctxbeg(self): return _manatee.KWICLines_get_ctxbeg(self)
    def get_ctxend(self): return _manatee.KWICLines_get_ctxend(self)
    def get_ref_list(self): return _manatee.KWICLines_get_ref_list(self)
    def get_refs(self): return _manatee.KWICLines_get_refs(self)
    def get_left(self): return _manatee.KWICLines_get_left(self)
    def get_kwic(self): return _manatee.KWICLines_get_kwic(self)
    def get_right(self): return _manatee.KWICLines_get_right(self)
    def get_linegroup(self): return _manatee.KWICLines_get_linegroup(self)
    __swig_destroy__ = _manatee.delete_KWICLines
    __del__ = lambda self : None;
KWICLines_swigregister = _manatee.KWICLines_swigregister
KWICLines_swigregister(KWICLines)

class CollocItems(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CollocItems, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CollocItems, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _manatee.new_CollocItems(*args)
        try: self.this.append(this)
        except: self.this = this
    def next(self): return _manatee.CollocItems_next(self)
    def eos(self): return _manatee.CollocItems_eos(self)
    def get_item(self): return _manatee.CollocItems_get_item(self)
    def get_freq(self): return _manatee.CollocItems_get_freq(self)
    def get_cnt(self): return _manatee.CollocItems_get_cnt(self)
    def get_bgr(self, *args): return _manatee.CollocItems_get_bgr(self, *args)
    __swig_destroy__ = _manatee.delete_CollocItems
    __del__ = lambda self : None;
CollocItems_swigregister = _manatee.CollocItems_swigregister
CollocItems_swigregister(CollocItems)

class CorpRegion(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CorpRegion, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CorpRegion, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _manatee.new_CorpRegion(*args)
        try: self.this.append(this)
        except: self.this = this
    def region(self, *args): return _manatee.CorpRegion_region(self, *args)
    __swig_destroy__ = _manatee.delete_CorpRegion
    __del__ = lambda self : None;
CorpRegion_swigregister = _manatee.CorpRegion_swigregister
CorpRegion_swigregister(CorpRegion)

class SubCorpus(Corpus):
    __swig_setmethods__ = {}
    for _s in [Corpus]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, SubCorpus, name, value)
    __swig_getmethods__ = {}
    for _s in [Corpus]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, SubCorpus, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _manatee.new_SubCorpus(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _manatee.delete_SubCorpus
    __del__ = lambda self : None;
SubCorpus_swigregister = _manatee.SubCorpus_swigregister
SubCorpus_swigregister(SubCorpus)


def create_subcorpus(*args):
  return _manatee.create_subcorpus(*args)
create_subcorpus = _manatee.create_subcorpus

def find_subcorpora(*args):
  return _manatee.find_subcorpora(*args)
find_subcorpora = _manatee.find_subcorpora
class map_int_sort_bigrams(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, map_int_sort_bigrams, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, map_int_sort_bigrams, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _manatee.new_map_int_sort_bigrams(*args)
        try: self.this.append(this)
        except: self.this = this
    def maxid(self): return _manatee.map_int_sort_bigrams_maxid(self)
    def count(self, *args): return _manatee.map_int_sort_bigrams_count(self, *args)
    def value(self, *args): return _manatee.map_int_sort_bigrams_value(self, *args)
    __swig_destroy__ = _manatee.delete_map_int_sort_bigrams
    __del__ = lambda self : None;
map_int_sort_bigrams_swigregister = _manatee.map_int_sort_bigrams_swigregister
map_int_sort_bigrams_swigregister(map_int_sort_bigrams)


def compute_ARF(*args):
  return _manatee.compute_ARF(*args)
compute_ARF = _manatee.compute_ARF

def compute_fALD(*args):
  return _manatee.compute_fALD(*args)
compute_fALD = _manatee.compute_fALD

def estimate_colloc_poss(*args):
  return _manatee.estimate_colloc_poss(*args)
estimate_colloc_poss = _manatee.estimate_colloc_poss
class CorpInfo(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CorpInfo, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CorpInfo, name)
    __repr__ = _swig_repr
    def dump(self, indent=0): return _manatee.CorpInfo_dump(self, indent)
    def add_attr(self, *args): return _manatee.CorpInfo_add_attr(self, *args)
    def add_struct(self, *args): return _manatee.CorpInfo_add_struct(self, *args)
    def set_opt(self, *args): return _manatee.CorpInfo_set_opt(self, *args)
    def find_opt(self, *args): return _manatee.CorpInfo_find_opt(self, *args)
    def __init__(self): 
        this = _manatee.new_CorpInfo()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _manatee.delete_CorpInfo
    __del__ = lambda self : None;
CorpInfo_swigregister = _manatee.CorpInfo_swigregister
CorpInfo_swigregister(CorpInfo)


def loadCorpInfo(*args):
  return _manatee.loadCorpInfo(*args)
loadCorpInfo = _manatee.loadCorpInfo
# This file is compatible with both classic and new-style classes.


