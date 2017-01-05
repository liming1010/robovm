/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.8
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.robovm.llvm.binding;

public class IntArray {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected IntArray(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(IntArray obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        LLVMJNI.delete_IntArray(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setValue(int value) {
    LLVMJNI.IntArray_value_set(swigCPtr, this, value);
  }

  public int getValue() {
    return LLVMJNI.IntArray_value_get(swigCPtr, this);
  }

  public IntArray(int nelements) {
    this(LLVMJNI.new_IntArray(nelements), true);
  }

  public int get(int index) {
    return LLVMJNI.IntArray_get(swigCPtr, this, index);
  }

  public void set(int index, int value) {
    LLVMJNI.IntArray_set(swigCPtr, this, index, value);
  }

}
