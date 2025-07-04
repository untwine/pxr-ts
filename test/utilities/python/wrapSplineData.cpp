// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <tsTest/splineData.h>
#include <pxr/tf/enum.h>
#include <pxr/tf/pyEnum.h>
#include <pxr/tf/pyContainerConversions.h>
#include <pxr/tf/pyResultConversions.h>
#include <pxr/tf/stringUtils.h>
#include <pxr/tf/diagnostic.h>

#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/make_constructor.hpp>
#include <pxr/boost/python/operators.hpp>
#include <sstream>
#include <string>
#include <cstdio>

using namespace pxr;

using namespace pxr::boost::python;

using This = TsTest_SplineData;


#define SET_MEMBER(result, type, member, value)                   \
    if (!value.is_none())                                         \
    {                                                             \
        extract<type> extractor(value);                           \
        if (extractor.check())                                    \
        {                                                         \
            result->member = extractor();                         \
        }                                                         \
        else                                                      \
        {                                                         \
            TF_CODING_ERROR("Unexpected type for " #member);      \
        }                                                         \
    }

#define SET_METHOD(result, type, method, value)                   \
    if (!value.is_none())                                         \
    {                                                             \
        extract<type> extractor(value);                           \
        if (extractor.check())                                    \
        {                                                         \
            result->method(extractor());                          \
        }                                                         \
        else                                                      \
        {                                                         \
            TF_CODING_ERROR("Unexpected type for " #method);      \
        }                                                         \
    }

// Return a full-precision python repr for a double value.
static std::string
_HexFloatRepr(const double num)
{
    // XXX: work around std::hexfloat apparently not working in our libstdc++ as
    // of this writing.
    char buf[100];
    snprintf(buf, sizeof(buf), "float.fromhex('%a')", num);
    return std::string(buf);
}

static This::Knot*
_ConstructKnot(
    const double timeIn,
    const This::InterpMethod nextSegInterpMethodIn,
    const double valueIn,
    const object &preValueIn,
    const object &preSlopeIn,
    const object &postSlopeIn,
    const object &preLenIn,
    const object &postLenIn,
    const object &preAutoIn,
    const object &postAutoIn)
{
    This::Knot *result = new This::Knot();

    result->time = timeIn;
    result->nextSegInterpMethod = nextSegInterpMethodIn;
    result->value = valueIn;

    SET_MEMBER(result, double, preValue, preValueIn);
    SET_MEMBER(result, double, preSlope, preSlopeIn);
    SET_MEMBER(result, double, postSlope, postSlopeIn);
    SET_MEMBER(result, double, preLen, preLenIn);
    SET_MEMBER(result, double, postLen, postLenIn);
    SET_MEMBER(result, bool, preAuto, preAutoIn);
    SET_MEMBER(result, bool, postAuto, postAutoIn);

    if (!preValueIn.is_none())
    {
        result->isDualValued = true;
    }

    return result;
}

static std::string
_KnotRepr(const This::Knot &kf)
{
    std::ostringstream result;
    result << "ts_test.TsTest_SplineData.Knot("
           << "time = " << _HexFloatRepr(kf.time)
           << ", nextSegInterpMethod = ts_test.TsTest_SplineData."
           << TfEnum::GetName(kf.nextSegInterpMethod)
           << ", value = " << _HexFloatRepr(kf.value)
           << ", preSlope = " << _HexFloatRepr(kf.preSlope)
           << ", postSlope = " << _HexFloatRepr(kf.postSlope)
           << ", preLen = " << _HexFloatRepr(kf.preLen)
           << ", postLen = " << _HexFloatRepr(kf.postLen)
           << ", preAuto = " << (kf.preAuto ? "True" : "False")
           << ", postAuto = " << (kf.postAuto ? "True" : "False");

    if (kf.isDualValued)
    {
        result << ", preValue = " << _HexFloatRepr(kf.preValue);
    }

    result << ")";

    return result.str();
}

static This::InnerLoopParams*
_ConstructInnerLoopParams(
    const bool enabledIn,
    const object &protoStartIn,
    const object &protoEndIn,
    const object &numPreLoopsIn,
    const object &numPostLoopsIn,
    const object &valueOffsetIn)
{
    This::InnerLoopParams *result = new This::InnerLoopParams();

    result->enabled = enabledIn;

    SET_MEMBER(result, double, protoStart, protoStartIn);
    SET_MEMBER(result, double, protoEnd, protoEndIn);
    SET_MEMBER(result, int, numPreLoops, numPreLoopsIn);
    SET_MEMBER(result, int, numPostLoops, numPostLoopsIn);
    SET_MEMBER(result, double, valueOffset, valueOffsetIn);

    return result;
}

static std::string
_InnerLoopParamsRepr(const This::InnerLoopParams &lp)
{
    std::ostringstream result;

    result << "ts_test.TsTest_SplineData.InnerLoopParams("
           << "enabled = " << (lp.enabled ? "True" : "False")
           << ", protoStart = " << _HexFloatRepr(lp.protoStart)
           << ", protoEnd = " << _HexFloatRepr(lp.protoEnd)
           << ", numPreLoops = " << lp.numPreLoops
           << ", numPostLoops = " << lp.numPostLoops
           << ", valueOffset = " << _HexFloatRepr(lp.valueOffset)
           << ")";

    return result.str();
}

static This::Extrapolation*
_ConstructExtrapolation(
    const This::ExtrapMethod method,
    const double slope,
    const This::LoopMode loopMode)
{
    This::Extrapolation *result = new This::Extrapolation();

    result->method = method;
    result->slope = slope;
    result->loopMode = loopMode;

    return result;
}

static std::string
_ExtrapolationRepr(const This::Extrapolation &e)
{
    std::ostringstream result;

    result << "ts_test.TsTest_SplineData.Extrapolation("
           << "method = ts_test.TsTest_SplineData." << TfEnum::GetName(e.method);

    if (e.method == This::ExtrapSloped)
    {
        result << ", slope = " << _HexFloatRepr(e.slope);
    }
    else if (e.method == This::ExtrapLoop)
    {
        result << ", loopMode = ts_test.TsTest_SplineData."
               << TfEnum::GetName(e.loopMode);
    }

    result << ")";

    return result.str();
}

static void _SetKnots(
    This &data,
    const object &knots)
{
    // Convert list-of-knots to std::set-of-knots.
    if (!knots.is_none())
    {
        extract<std::vector<This::Knot>> extractor(knots);
        if (extractor.check())
        {
            std::vector<This::Knot> knotVec = extractor();
            This::KnotSet knotSet(knotVec.begin(), knotVec.end());
            data.SetKnots(knotSet);
        }
        else
        {
            TF_CODING_ERROR("Unexpected type for SetKnots");
        }
    }
}

static This*
_ConstructSplineData(
    const bool isHermite,
    const object &knots,
    const object &preExtrap,
    const object &postExtrap,
    const object &loopParams)
{
    This *result = new This();

    result->SetIsHermite(isHermite);

    // Convert list-of-knots to std::set-of-knots.
    _SetKnots(*result, knots);

    SET_METHOD(result, This::InnerLoopParams, SetInnerLoopParams, loopParams);
    SET_METHOD(result, This::Extrapolation, SetPreExtrapolation, preExtrap);
    SET_METHOD(result, This::Extrapolation, SetPostExtrapolation, postExtrap);

    return result;
}

static std::string
_SplineDataRepr(const This &data)
{
    std::ostringstream result;

    result << "ts_test.TsTest_SplineData("
           << "isHermite = " << (data.GetIsHermite() ? "True" : "False")
           << ", preExtrapolation = "
           << _ExtrapolationRepr(data.GetPreExtrapolation())
           << ", postExtrapolation = "
           << _ExtrapolationRepr(data.GetPostExtrapolation());

    const This::KnotSet &knots = data.GetKnots();
    if (!knots.empty())
    {
        std::vector<std::string> kfStrs;
        for (const This::Knot &kf : knots)
            kfStrs.push_back(_KnotRepr(kf));

        // Expose knots as a list, even though in C++ they're a std::set.
        // We want them sorted, which Python sets are not.
        result << ", knots = [" << TfStringJoin(kfStrs, ", ") << "]";
    }

    if (data.GetInnerLoopParams().enabled)
    {
        result << ", innerLoopParams = "
               << _InnerLoopParamsRepr(data.GetInnerLoopParams());
    }

    result << ")";

    return result.str();
}

static std::string
_SplineDataDebugDescription(const This &data)
{
    return data.GetDebugDescription();
}

void wrapTsTest_SplineData()
{
    // First the class object, so we can create a scope for it...
    class_<This> classObj("TsTest_SplineData", no_init);
    scope classScope(classObj);

    // ...then the nested type wrappings, which require the scope...

    TfPyWrapEnum<This::InterpMethod>();
    TfPyWrapEnum<This::ExtrapMethod>();
    TfPyWrapEnum<This::LoopMode>();
    TfPyWrapEnum<This::Feature>();

    class_<This::Knot>("Knot", no_init)
        .def(init<const This::Knot&>())
        .def("__init__",
            make_constructor(
                &_ConstructKnot, default_call_policies(), (
                    arg("time") = 0.0,
                    arg("nextSegInterpMethod") = This::InterpHeld,
                    arg("value") = 0.0,
                    arg("preValue") = object(),
                    arg("preSlope") = object(),
                    arg("postSlope") = object(),
                    arg("preLen") = object(),
                    arg("postLen") = object(),
                    arg("preAuto") = object(),
                    arg("postAuto") = object()
                )))
        .def("__repr__", &_KnotRepr)
        .def(self == self)
        .def(self != self)
        .def(self < self)
        .def_readwrite("time", &This::Knot::time)
        .def_readwrite(
            "nextSegInterpMethod", &This::Knot::nextSegInterpMethod)
        .def_readwrite("value", &This::Knot::value)
        .def_readwrite("isDualValued", &This::Knot::isDualValued)
        .def_readwrite("preValue", &This::Knot::preValue)
        .def_readwrite("preSlope", &This::Knot::preSlope)
        .def_readwrite("postSlope", &This::Knot::postSlope)
        .def_readwrite("preLen", &This::Knot::preLen)
        .def_readwrite("postLen", &This::Knot::postLen)
        .def_readwrite("preAuto", &This::Knot::preAuto)
        .def_readwrite("postAuto", &This::Knot::postAuto)
        ;

    class_<This::InnerLoopParams>("InnerLoopParams", no_init)
        .def(init<const This::InnerLoopParams&>())
        .def("__init__",
            make_constructor(
                &_ConstructInnerLoopParams, default_call_policies(), (
                    arg("enabled") = false,
                    arg("protoStart") = object(),
                    arg("protoEnd") = object(),
                    arg("numPreLoops") = object(),
                    arg("numPostLoops") = object(),
                    arg("valueOffset") = object()
                )))
        .def("__repr__", &_InnerLoopParamsRepr)
        .def(self == self)
        .def(self != self)
        .def_readwrite("enabled", &This::InnerLoopParams::enabled)
        .def_readwrite("protoStart", &This::InnerLoopParams::protoStart)
        .def_readwrite("protoEnd", &This::InnerLoopParams::protoEnd)
        .def_readwrite("numPreLoops", &This::InnerLoopParams::numPreLoops)
        .def_readwrite("numPostLoops", &This::InnerLoopParams::numPostLoops)
        .def_readwrite("valueOffset", &This::InnerLoopParams::valueOffset)
        .def("IsValid", &This::InnerLoopParams::IsValid)
        ;

    class_<This::Extrapolation>("Extrapolation", no_init)
        .def(init<const This::Extrapolation&>())
        .def("__init__",
            make_constructor(
                &_ConstructExtrapolation, default_call_policies(), (
                    arg("method") = This::ExtrapHeld,
                    arg("slope") = 0.0,
                    arg("loopMode") = This::LoopNone
                )))
        .def("__repr__", &_ExtrapolationRepr)
        .def(self == self)
        .def(self != self)
        .def_readwrite("method", &This::Extrapolation::method)
        .def_readwrite("slope", &This::Extrapolation::slope)
        .def_readwrite("loopMode", &This::Extrapolation::loopMode)
        ;

    TfPyRegisterStlSequencesFromPython<double>();
    TfPyRegisterStlSequencesFromPython<This::Knot>();

    // ...then the defs, which must occur after the nested type wrappings.
    classObj

        .def(init<const This&>())
        .def("__init__",
            make_constructor(
                &_ConstructSplineData, default_call_policies(), (
                    arg("isHermite") = false,
                    arg("knots") = object(),
                    arg("preExtrapolation") = object(),
                    arg("postExtrapolation") = object(),
                    arg("innerLoopParams") = object()
                )))

        .def("__repr__", &_SplineDataRepr)
        .def("__str__", &_SplineDataDebugDescription)

        .def(self == self)
        .def(self != self)

        .def("SetIsHermite", &This::SetIsHermite,
            (arg("isHermite")))

        .def("AddKnot", &This::AddKnot,
            (arg("knot")))

        .def("SetKnots", &_SetKnots,
            (arg("knots")))

        .def("SetPreExtrapolation", &This::SetPreExtrapolation,
            (arg("preExtrap")))

        .def("SetPostExtrapolation", &This::SetPostExtrapolation,
            (arg("postExtrap")))

        .def("SetInnerLoopParams", &This::SetInnerLoopParams,
            (arg("params")))

        .def("GetIsHermite", &This::GetIsHermite)

        .def("GetKnots", &This::GetKnots,
            return_value_policy<TfPySequenceToList>())

        .def("GetPreExtrapolation", &This::GetPreExtrapolation,
            return_value_policy<return_by_value>())

        .def("GetPostExtrapolation", &This::GetPostExtrapolation,
            return_value_policy<return_by_value>())

        .def("GetInnerLoopParams", &This::GetInnerLoopParams,
            return_value_policy<return_by_value>())

        .def("GetRequiredFeatures", &This::GetRequiredFeatures)

        .def("GetDebugDescription", &This::GetDebugDescription,
            (arg("precision") = 6))

        ;
}
