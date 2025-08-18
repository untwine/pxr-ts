//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include <pxr/ts/pxr.h>
#include <tsTest/tsEvaluator.h>
#include <pxr/ts/spline.h>
#include <tsTest/sampleTimes.h>

#include <pxr/tf/pyResultConversions.h>

#include <pxr/boost/python/class.hpp>

TS_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;

static object
_wrapSample(const TsTest_TsEvaluator& evaluator,
            const TsSpline& spline,
            const GfInterval& timeInterval,
            const double timeScale,
            const double valueScale,
            const double tolerance,
            bool withSamples = false)
{
    if (withSamples) {
        TsSplineSamplesWithSources<GfVec2d> samples;
        if (evaluator.Sample(spline, timeInterval,
                             timeScale, valueScale, tolerance,
                             &samples)) {
            return object(samples);
        }
    } else {
        TsSplineSamples<GfVec2d> samples;
        if (evaluator.Sample(spline, timeInterval,
                             timeScale, valueScale, tolerance,
                             &samples)) {
            return object(samples);
        }
    }

    return object();
}

void wrapTsTest_TsEvaluator()
{
    using This = TsTest_TsEvaluator;

    class_<This>("TsTest_TsEvaluator")
        // Default init is not suppressed, so automatically created.

        .def("Eval", &This::Eval,
            (arg("spline"),
             arg("sampleTimes")),
            return_value_policy<TfPySequenceToList>())

        .def("Sample", &_wrapSample,
            (arg("spline"),
             arg("timeInterval"),
             arg("timeScale"),
             arg("valueScale"),
             arg("tolerance"),
             arg("withSources") = false))

        /*
        .def("BakeInnerLoops", &This::BakeInnerLoops,
            (arg("splineData")))
        */

        ;
}
