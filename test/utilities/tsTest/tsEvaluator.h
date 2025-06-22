// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TS_TS_TEST_TS_EVALUATOR_H
#define PXR_TS_TS_TEST_TS_EVALUATOR_H

#include "./api.h"
#include <pxr/ts/types.h>
#include "./types.h"

#include <pxr/tf/type.h>

namespace pxr {

class TsTest_SplineData;
class TsTest_SampleTimes;
class TsSpline;
class GfInterval;

// Perform test evaluation using Ts.
//
class TsTest_TsEvaluator
{
public:
    ////////////////////////////////////////////////////////////////////////////
    // EVALUATION

    // Evaluate at specified times.
    TS_TEST_API
    TsTest_SampleVec Eval(
        const TsTest_SplineData &splineData,
        const TsTest_SampleTimes &sampleTimes) const;

    // Produce bulk samples for drawing.  Sample times are determined adaptively
    // and cannot be controlled.
    template <typename SampleData>
    bool Sample(
        const TsTest_SplineData &splineData,
        const GfInterval& timeInterval,
        double timeScale,
        double valueScale,
        double tolerance,
        SampleData* splineSamples) const;

    ////////////////////////////////////////////////////////////////////////////
    // CONVERSION

    // Convert a TsSpline into TsTest's SplineData form.
    TS_TEST_API
    TsTest_SplineData SplineToSplineData(
        const TsSpline &spline) const;

    // Convert SplineData to a TsSpline with double values
    TS_TEST_API
    TsSpline SplineDataToSpline(
        const TsTest_SplineData &splineData) const;
 
    // Convert SplineData to a TsSpline with valueType values
   TS_TEST_API
    TsSpline SplineDataToSpline(
        const TsTest_SplineData &splineData,
        const TfType& valueType) const;

    ////////////////////////////////////////////////////////////////////////////
    // TEST DATA TRANSFORMATION

    // Produce a copy of splineData with inner loops, if any, baked out into
    // ordinary knots.
    /*
    TS_TEST_API
    TsTest_SplineData BakeInnerLoops(
        const TsTest_SplineData &splineData) const;
    */
};

}  // namespace pxr

#endif
