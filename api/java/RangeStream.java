/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sketchengine.manatee;

public class RangeStream {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected RangeStream(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(RangeStream obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        manateeJNI.delete_RangeStream(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public boolean next() {
    return manateeJNI.RangeStream_next(swigCPtr, this);
  }

  public boolean end() {
    return manateeJNI.RangeStream_end(swigCPtr, this);
  }

  public long peek_beg() {
    return manateeJNI.RangeStream_peek_beg(swigCPtr, this);
  }

  public long peek_end() {
    return manateeJNI.RangeStream_peek_end(swigCPtr, this);
  }

  public long find_beg(long pos) {
    return manateeJNI.RangeStream_find_beg(swigCPtr, this, pos);
  }

  public long find_end(long pos) {
    return manateeJNI.RangeStream_find_end(swigCPtr, this, pos);
  }

  public long count_rest() {
    return manateeJNI.RangeStream_count_rest(swigCPtr, this);
  }

  public void collocs(IntVector colls) {
    manateeJNI.RangeStream_collocs(swigCPtr, this, IntVector.getCPtr(colls), colls);
  }

}
