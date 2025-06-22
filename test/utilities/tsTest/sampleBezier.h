// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TS_TS_TEST_SAMPLE_BEZIER_H
#define PXR_TS_TS_TEST_SAMPLE_BEZIER_H

#include "./api.h"
#include "./splineData.h"
#include "./types.h"

#include <vector>

namespace pxr {

// Produces (time, value) samples along a Bezier curve by walking the 't'
// parameter space.  The samples are evenly divided among the segments, and then
// uniformly in the 't' parameter for each segment.  Samples do not necessarily
// always go forward in time; Bezier segments may form loops that temporarily
// reverse direction.
//
// Only Bezier segments are supported.  No extrapolation is performed.
//
TS_TEST_API
TsTest_SampleVec
TsTest_SampleBezier(
    const TsTest_SplineData &splineData,
    int numSamples);

}  // namespace pxr

#endif
