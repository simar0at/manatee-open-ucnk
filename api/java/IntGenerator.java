/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sketchengine.manatee;

public class IntGenerator {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public IntGenerator(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(IntGenerator obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        manateeJNI.delete_IntGenerator(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public int next() {
    return manateeJNI.IntGenerator_next(swigCPtr, this);
  }

  public boolean end() {
    return manateeJNI.IntGenerator_end(swigCPtr, this);
  }

}
