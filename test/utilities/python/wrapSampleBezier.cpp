//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include <pxr/ts/pxr.h>
#include <tsTest/sampleBezier.h>
#include <pxr/tf/pyResultConversions.h>

#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/return_value_policy.hpp>

TS_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;


void wrapTsTest_SampleBezier()
{
    def("TsTest_SampleBezier", &TsTest_SampleBezier,
        (arg("splineData"),
         arg("numSamples")),
        return_value_policy<TfPySequenceToList>());
}
