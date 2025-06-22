// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <tsTest/types.h>
#include <pxr/tf/pyContainerConversions.h>

#include <pxr/boost/python/class.hpp>
#include <sstream>
#include <string>
#include <cstdio>

using namespace pxr;

using namespace pxr::boost::python;


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

static std::string
_SampleRepr(const TsTest_Sample &sample)
{
    std::ostringstream result;

    result << "ts_test.TsTest_Sample("
           << _HexFloatRepr(sample.time)
           << ", " << _HexFloatRepr(sample.value)
           << ")";

    return result.str();
}


void wrapTsTest_Types()
{
    class_<TsTest_Sample>("TsTest_Sample")
        // Default init is not suppressed, so automatically created.
        .def(init<double, double>())
        .def(init<const TsTest_Sample&>())
        .def("__repr__", &_SampleRepr)
        .def_readwrite("time", &TsTest_Sample::time)
        .def_readwrite("value", &TsTest_Sample::value)
        ;

    to_python_converter<
        TsTest_SampleVec,
        TfPySequenceToPython<TsTest_SampleVec>>();
    TfPyContainerConversions::from_python_sequence<
        TsTest_SampleVec,
        TfPyContainerConversions::variable_capacity_policy>();
}
