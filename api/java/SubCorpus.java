/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sketchengine.manatee;

public class SubCorpus extends Corpus {
  private long swigCPtr;

  protected SubCorpus(long cPtr, boolean cMemoryOwn) {
    super(manateeJNI.SubCorpus_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(SubCorpus obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        manateeJNI.delete_SubCorpus(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public SubCorpus(Corpus corp, String sub, boolean complement) {
    this(manateeJNI.new_SubCorpus__SWIG_0(Corpus.getCPtr(corp), corp, sub, complement), true);
  }

  public SubCorpus(Corpus corp, String sub) {
    this(manateeJNI.new_SubCorpus__SWIG_1(Corpus.getCPtr(corp), corp, sub), true);
  }

}
