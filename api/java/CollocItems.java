/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sketchengine.manatee;

public class CollocItems {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected CollocItems(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(CollocItems obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        manateeJNI.delete_CollocItems(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public CollocItems(Concordance conc, String attr_name, char sort_fun_code, long minfreq, long minbgr, int fromw, int tow, int maxitems) {
    this(manateeJNI.new_CollocItems(Concordance.getCPtr(conc), conc, attr_name, sort_fun_code, minfreq, minbgr, fromw, tow, maxitems), true);
  }

  public void next() {
    manateeJNI.CollocItems_next(swigCPtr, this);
  }

  public boolean eos() {
    return manateeJNI.CollocItems_eos(swigCPtr, this);
  }

  public String get_item() {
    return manateeJNI.CollocItems_get_item(swigCPtr, this);
  }

  public long get_freq() {
    return manateeJNI.CollocItems_get_freq(swigCPtr, this);
  }

  public long get_cnt() {
    return manateeJNI.CollocItems_get_cnt(swigCPtr, this);
  }

  public double get_bgr(char bgr_code) {
    return manateeJNI.CollocItems_get_bgr(swigCPtr, this, bgr_code);
  }

}
