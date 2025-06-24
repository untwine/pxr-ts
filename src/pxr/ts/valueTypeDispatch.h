// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TS_VALUE_TYPE_DISPATCH_H
#define PXR_TS_VALUE_TYPE_DISPATCH_H

#include "./typeHelpers.h"
#include <pxr/gf/half.h>
#include <pxr/tf/type.h>
#include <pxr/tf/diagnostic.h>

#include <utility>

namespace pxr {


// Makes a call to a template functor based on a dynamic type.  No return value;
// obtain outputs with out-params.  Supports all valid spline value types.
//
// Example:
//
//   template <typename T>
//   struct _HasNonzeroValue
//   {
//       void operator()(const TsKnot &knot, bool *resultOut)
//       {
//           T value = 0;
//           if (knot.GetValue(&value))
//               *resultOut = (value != 0);
//           else
//               *resultOut = false;
//       }
//   };
//
//   bool nonzero = false;
//   TsDispatchToValueTypeTemplate<_HasNonzeroValue>(
//       myKnot.GetValueType(), myKnot, &nonzero);
//
template <
    template <typename T> class Cls,
    typename... Args>
void TsDispatchToValueTypeTemplate(
    TfType valueType, Args&&... args)
{
#define _MAKE_CLAUSE(unused, tuple)                                         \
if (valueType == Ts_GetType<TS_SPLINE_VALUE_CPP_TYPE(tuple)>())             \
{                                                                           \
    Cls<TS_SPLINE_VALUE_CPP_TYPE(tuple)>()(std::forward<Args>(args)...);    \
    return;                                                                 \
}
TF_PP_SEQ_FOR_EACH(_MAKE_CLAUSE, ~, TS_SPLINE_SUPPORTED_VALUE_TYPES)
    TF_CODING_ERROR("Unsupported spline value type");
}


}  // namespace pxr

#endif
