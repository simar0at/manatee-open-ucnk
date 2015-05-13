/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sketchengine.manatee;

public class StrVector {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public StrVector(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(StrVector obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        manateeJNI.delete_StrVector(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public StrVector() {
    this(manateeJNI.new_StrVector__SWIG_0(), true);
  }

  public StrVector(long n) {
    this(manateeJNI.new_StrVector__SWIG_1(n), true);
  }

  public long size() {
    return manateeJNI.StrVector_size(swigCPtr, this);
  }

  public long capacity() {
    return manateeJNI.StrVector_capacity(swigCPtr, this);
  }

  public void reserve(long n) {
    manateeJNI.StrVector_reserve(swigCPtr, this, n);
  }

  public boolean isEmpty() {
    return manateeJNI.StrVector_isEmpty(swigCPtr, this);
  }

  public void clear() {
    manateeJNI.StrVector_clear(swigCPtr, this);
  }

  public void add(String x) {
    manateeJNI.StrVector_add(swigCPtr, this, x);
  }

  public String get(int i) {
    return manateeJNI.StrVector_get(swigCPtr, this, i);
  }

  public void set(int i, String val) {
    manateeJNI.StrVector_set(swigCPtr, this, i, val);
  }

}
