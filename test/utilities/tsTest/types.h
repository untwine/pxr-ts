// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TS_TS_TEST_TYPES_H
#define PXR_TS_TS_TEST_TYPES_H

#include "./api.h"

#include <vector>

namespace pxr {

class TsTest_Sample
{
public:
    double time = 0;
    double value = 0;

public:
    TsTest_Sample() = default;

    TsTest_Sample(double timeIn, double valueIn)
        : time(timeIn), value(valueIn) {}
};

using TsTest_SampleVec = std::vector<TsTest_Sample>;

}  // namespace pxr

#endif
