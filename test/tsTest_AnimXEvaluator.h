// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TS_TS_TEST_ANIM_X_EVALUATOR_H
#define PXR_TS_TS_TEST_ANIM_X_EVALUATOR_H

#include <pxr/ts/api.h>
#include "./tsTest_Types.h"

namespace pxr {

class TsTest_SplineData;
class TsTest_SampleTimes;

class TsTest_AnimXEvaluator
{
public:
    enum AutoTanType
    {
        AutoTanAuto,
        AutoTanSmooth
    };

    TS_API
    TsTest_AnimXEvaluator(
        AutoTanType autoTanType = AutoTanAuto);

    TS_API
    TsTest_SampleVec Eval(
        const TsTest_SplineData &splineData,
        const TsTest_SampleTimes &sampleTimes) const;

private:
    const AutoTanType _autoTanType;
};

}  // namespace pxr

#endif
