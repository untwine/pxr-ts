//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include <pxr/tf/pyModule.h>

using namespace pxr;

TF_WRAP_MODULE
{
    TF_WRAP(TsTest_Museum);
    TF_WRAP(TsTest_SampleBezier);
    TF_WRAP(TsTest_SampleTimes);
    TF_WRAP(TsTest_TsEvaluator);
    TF_WRAP(TsTest_Types);
}
