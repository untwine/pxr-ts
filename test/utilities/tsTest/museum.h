// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TS_TS_TEST_MUSEUM_H
#define PXR_TS_TS_TEST_MUSEUM_H

#include "./api.h"
#include "./splineData.h"

#include <vector>
#include <string>

namespace pxr {


// A collection of museum exhibits.  These are spline cases that can be used by
// tests to exercise various behaviors.
//
class TsTest_Museum
{
public:
    enum DataId
    {
        // Single-segment ordinary curves.
        TwoKnotBezier,
        TwoKnotLinear,

        // Longer splines.
        FourKnotBezier,

        // Looping cases.
        SimpleInnerLoop,
        InnerLoop2and2,
        InnerLoopPre,
        InnerLoopPost,
        ExtrapLoopRepeat,
        ExtrapLoopReset,
        ExtrapLoopOscillate,
        InnerAndExtrapLoops,

        // Tests of several regressive cases.
        RegressiveLoop,
        RegressiveS,
        RegressiveSStandard,
        RegressiveSPreOut,
        RegressiveSPostOut,
        RegressiveSBothOut,
        RegressivePreJ,
        RegressivePostJ,
        RegressivePreC,
        RegressivePostC,
        RegressivePreG,
        RegressivePostG,
        RegressivePreFringe,
        RegressivePostFringe,

        // Bold case: escaped tangents, but not regressive.
        BoldS,

        // Edge case: cusp.  Valid but just barely; undefined tangent.
        Cusp,

        // Edge case: vertical tangent in center.  Also a less extreme variant.
        CenterVertical,
        NearCenterVertical,

        // A case that hit an old bug.  A particular case of a single vertical.
        VerticalTorture,

        // Edge case: 4/3 + 1/3 tangents.  Vertical at 24/27.
        // Also the inverse.
        FourThirdOneThird,
        OneThirdFourThird,

        // Edge cases: single verticals at start and end.
        StartVert,
        EndVert,

        // Fringe vertical between FourThirdOneThird and EndVert.
        FringeVert,

        // N-shape, with near-vertical tangents.
        MarginalN,

        // Both tangents zero-length.
        ZeroTans,

        // Exercise many features of the object model.
        ComplexParams
    };

    // Get a case by ID.
    TS_TEST_API
    static TsTest_SplineData GetData(DataId id);

    // Get all case names.
    TS_TEST_API
    static std::vector<std::string> GetAllNames();

    // Get a case by name.
    TS_TEST_API
    static TsTest_SplineData GetDataByName(const std::string &name);
};


}  // namespace pxr


#endif
