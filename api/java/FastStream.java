/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sketchengine.manatee;

public class FastStream {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public FastStream(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(FastStream obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        manateeJNI.delete_FastStream(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public long next() {
    return manateeJNI.FastStream_next(swigCPtr, this);
  }

  public long peek() {
    return manateeJNI.FastStream_peek(swigCPtr, this);
  }

  public long find(long pos) {
    return manateeJNI.FastStream_find(swigCPtr, this, pos);
  }

  public boolean end() {
    return manateeJNI.FastStream_end(swigCPtr, this);
  }

}
