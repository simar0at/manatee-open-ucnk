/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sketchengine.manatee;

public class NGramIterator {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public NGramIterator(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(NGramIterator obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        manateeJNI.delete_NGramIterator(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public NGramIterator(SWIGTYPE_p_NGram__lex_iter iter) {
    this(manateeJNI.new_NGramIterator(SWIGTYPE_p_NGram__lex_iter.getCPtr(iter)), true);
  }

  public int next() {
    return manateeJNI.NGramIterator_next(swigCPtr, this);
  }

}