// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TS_EVAL_H
#define PXR_TS_EVAL_H

#include "./api.h"
#include "./types.h"

#include <optional>

namespace pxr {

struct Ts_SplineData;


enum Ts_EvalAspect
{
    Ts_EvalValue,
    Ts_EvalHeldValue,
    Ts_EvalDerivative
};

enum Ts_EvalLocation
{
    Ts_EvalPre,
    Ts_EvalAtTime,  // AtTime is implemented identically to Post,
                    // but the intent of Post is a limit, while AtTime is exact.
    Ts_EvalPost
};


// Evaluates a spline's value or derivative at a given time.  An empty return
// value means there is no value or derivative at all.
//
TS_API
std::optional<double>
Ts_Eval(
    const Ts_SplineData *data,
    TsTime time,
    Ts_EvalAspect aspect,
    Ts_EvalLocation location);


}  // namespace pxr

#endif
