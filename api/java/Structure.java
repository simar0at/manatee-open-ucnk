/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sketchengine.manatee;

public class Structure {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public Structure(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(Structure obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        manateeJNI.delete_Structure(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public long size() {
    return manateeJNI.Structure_size(swigCPtr, this);
  }

  public PosAttr get_attr(String attr_name) {
    long cPtr = manateeJNI.Structure_get_attr(swigCPtr, this, attr_name);
    return (cPtr == 0) ? null : new PosAttr(cPtr, false);
  }

  public String getName() {
    return manateeJNI.Structure_name_get(swigCPtr, this);
  }

  public long num_at_pos(long pos) {
    return manateeJNI.Structure_num_at_pos(swigCPtr, this, pos);
  }

  public long num_next_pos(long pos) {
    return manateeJNI.Structure_num_next_pos(swigCPtr, this, pos);
  }

  public long beg(long n) {
    return manateeJNI.Structure_beg(swigCPtr, this, n);
  }

  public long end(long n) {
    return manateeJNI.Structure_end(swigCPtr, this, n);
  }

  public RangeStream whole() {
    long cPtr = manateeJNI.Structure_whole(swigCPtr, this);
    return (cPtr == 0) ? null : new RangeStream(cPtr, true);
  }

  public RangeStream attr_val(String aname, int attr_id) {
    long cPtr = manateeJNI.Structure_attr_val(swigCPtr, this, aname, attr_id);
    return (cPtr == 0) ? null : new RangeStream(cPtr, true);
  }

}
