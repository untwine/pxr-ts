// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TS_TS_TEST_SAMPLE_TIMES_H
#define PXR_TS_TS_TEST_SAMPLE_TIMES_H

#include "./api.h"
#include "./splineData.h"

#include <vector>
#include <set>

namespace pxr {

class TsTest_SampleTimes
{
public:
    // A time at which to perform evaluation.  Typically just a time, but can
    // also be a "pre" time, which at a dual-valued knot can differ from the
    // ordinary value.
    class SampleTime
    {
    public:
        double time = 0.0;
        bool pre = false;

    public:
        TS_TEST_API
        SampleTime();

        TS_TEST_API
        SampleTime(double time);

        TS_TEST_API
        SampleTime(double time, bool pre);

        TS_TEST_API
        SampleTime& operator=(double time);

        TS_TEST_API
        bool operator<(const SampleTime &other) const;

        TS_TEST_API
        bool operator==(const SampleTime &other) const;

        TS_TEST_API
        bool operator!=(const SampleTime &other) const;
    };

    using SampleTimeSet = std::set<SampleTime>;

public:
    // DIRECT SPECIFICATION

    // Constructs a SampleTimes object for direct specification of times.
    TS_TEST_API
    TsTest_SampleTimes();

    // Adds the specified times.
    TS_TEST_API
    void AddTimes(
        const std::vector<double> &times);

    // Adds the specified times.
    TS_TEST_API
    void AddTimes(
        const std::vector<SampleTime> &times);

    // SPLINE-DRIVEN

    // Constructs a SampleTimes object for specification of times based on the
    // contents of splineData.
    TS_TEST_API
    TsTest_SampleTimes(
        const TsTest_SplineData &splineData);

    // Adds a time for each knot in splineData.  For dual-valued knots, adds
    // both a pre-time and an ordinary time.
    TS_TEST_API
    void AddKnotTimes();

    // Adds evenly spaced sample times within the frame range of splineData.
    // The first sample is after the first knot, and the last sample is before
    // the last knot.
    TS_TEST_API
    void AddUniformInterpolationTimes(
        int numSamples);

    // Determines the time range of the knots in splineData, extends it by
    // extrapolationFactor on each end, and adds one pre-extrapolating and one
    // post-extrapolating sample.  For example, with a time range of 10, and an
    // extrapolationFactor of 0.25, samples will be added 2.5 time units before
    // the first knot and 2.5 time units after the last.  For looping
    // extrapolation regions, this method does nothing; call
    // AddExtrapolatingLoopTimes instead or in addition.
    TS_TEST_API
    void AddExtrapolationTimes(
        double extrapolationFactor);

    // Adds times to handle extrapolating loops, if there are any.
    TS_TEST_API
    void AddExtrapolatingLoopTimes(
        int numIterations,
        int numSamplesPerIteration);

    // MACRO

    // Calls AddKnotTimes(), AddUniformInterpolationTimes(200),
    // AddExtrapolationTimes(0.2), and AddExtrapolatingLoopTimes(3, 200).
    TS_TEST_API
    void AddStandardTimes();

    // ACCESSORS

    // Returns the set of sample times.
    TS_TEST_API
    const SampleTimeSet&
    GetTimes() const;

    // Return the min time
    TS_TEST_API
    double
    GetMinTime() const;

    // Return the max time
    TS_TEST_API
    double
    GetMaxTime() const;

private:
    SampleTimeSet _GetKnotTimes() const;

private:
    const bool _haveSplineData;
    const TsTest_SplineData _splineData;

    SampleTimeSet _times;
};

}  // namespace pxr

#endif
