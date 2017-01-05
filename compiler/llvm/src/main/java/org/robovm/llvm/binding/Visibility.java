/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.8
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.robovm.llvm.binding;

public enum Visibility {
  DefaultVisibility,
  HiddenVisibility,
  ProtectedVisibility;

  public final int swigValue() {
    return swigValue;
  }

  public static Visibility swigToEnum(int swigValue) {
    Visibility[] swigValues = Visibility.class.getEnumConstants();
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (Visibility swigEnum : swigValues)
      if (swigEnum.swigValue == swigValue)
        return swigEnum;
    throw new IllegalArgumentException("No enum " + Visibility.class + " with value " + swigValue);
  }

  @SuppressWarnings("unused")
  private Visibility() {
    this.swigValue = SwigNext.next++;
  }

  @SuppressWarnings("unused")
  private Visibility(int swigValue) {
    this.swigValue = swigValue;
    SwigNext.next = swigValue+1;
  }

  @SuppressWarnings("unused")
  private Visibility(Visibility swigEnum) {
    this.swigValue = swigEnum.swigValue;
    SwigNext.next = this.swigValue+1;
  }

  private final int swigValue;

  private static class SwigNext {
    private static int next = 0;
  }
}

