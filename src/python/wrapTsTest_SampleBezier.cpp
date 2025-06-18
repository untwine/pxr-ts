// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/ts/tsTest_SampleBezier.h>
#include <pxr/tf/pyResultConversions.h>

#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/return_value_policy.hpp>

using namespace pxr;

using namespace pxr::boost::python;


void wrapTsTest_SampleBezier()
{
    def("TsTest_SampleBezier", &TsTest_SampleBezier,
        (arg("splineData"),
         arg("numSamples")),
        return_value_policy<TfPySequenceToList>());
}
