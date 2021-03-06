// Copyright (c) 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CSSValueIDMappings_h
#define CSSValueIDMappings_h

#include "core/CSSValueIDMappingsGenerated.h"

namespace blink {

template <class T>
T CssValueIDToPlatformEnum(CSSValueID v) {
  // By default, we use the generated mappings. For special cases, we
  // specialize.
  return detail::cssValueIDToPlatformEnumGenerated<T>(v);
}

template <class T>
inline CSSValueID PlatformEnumToCSSValueID(T v) {
  // By default, we use the generated mappings. For special cases, we overload.
  return detail::platformEnumToCSSValueIDGenerated(v);
}

template <>
inline UnicodeBidi CssValueIDToPlatformEnum(CSSValueID v) {
  if (v == CSSValueWebkitIsolate)
    return UnicodeBidi::kIsolate;
  if (v == CSSValueWebkitIsolateOverride)
    return UnicodeBidi::kIsolateOverride;
  if (v == CSSValueWebkitPlaintext)
    return UnicodeBidi::kPlaintext;
  return detail::cssValueIDToPlatformEnumGenerated<UnicodeBidi>(v);
}

template <>
inline ETextAlign CssValueIDToPlatformEnum(CSSValueID v) {
  if (v == CSSValueWebkitAuto)  // Legacy -webkit-auto. Eqiuvalent to start.
    return ETextAlign::kStart;
  if (v == CSSValueInternalCenter)
    return ETextAlign::kCenter;
  return detail::cssValueIDToPlatformEnumGenerated<ETextAlign>(v);
}

template <>
inline WritingMode CssValueIDToPlatformEnum(CSSValueID v) {
  switch (v) {
    case CSSValueHorizontalTb:
    case CSSValueLr:
    case CSSValueLrTb:
    case CSSValueRl:
    case CSSValueRlTb:
      return WritingMode::kHorizontalTb;
    case CSSValueVerticalRl:
    case CSSValueTb:
    case CSSValueTbRl:
      return WritingMode::kVerticalRl;
    case CSSValueVerticalLr:
      return WritingMode::kVerticalLr;
    default:
      break;
  }

  NOTREACHED();
  return WritingMode::kHorizontalTb;
}

template <>
inline ECursor CssValueIDToPlatformEnum(CSSValueID v) {
  if (v == CSSValueWebkitZoomIn)
    return ECursor::kZoomIn;
  if (v == CSSValueWebkitZoomOut)
    return ECursor::kZoomOut;
  return detail::cssValueIDToPlatformEnumGenerated<ECursor>(v);
}

template <>
inline EDisplay CssValueIDToPlatformEnum(CSSValueID v) {
  if (v == CSSValueWebkitFlex)
    return EDisplay::kFlex;
  if (v == CSSValueWebkitInlineFlex)
    return EDisplay::kInlineFlex;
  return detail::cssValueIDToPlatformEnumGenerated<EDisplay>(v);
}

}  // namespace blink

#endif
