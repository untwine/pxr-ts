//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include <pxr/ts/pxr.h>
#include <tsTest/museum.h>

#include <pxr/tf/pyEnum.h>
#include <pxr/tf/pyResultConversions.h>

#include <pxr/boost/python/class.hpp>

TS_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;

using This = TsTest_Museum;


void wrapTsTest_Museum()
{
    // First the class object, so we can create a scope for it...
    class_<This> classObj("TsTest_Museum", no_init);
    scope classScope(classObj);

    // ...then the nested type wrappings, which require the scope...
    TfPyWrapEnum<This::DataId>();

    // ...then the defs, which must occur after the nested type wrappings.
    classObj

        .def("GetAllNames", &This::GetAllNames,
            return_value_policy<TfPySequenceToList>())

        .def("GetSpline", &This::GetSpline,
            (arg("id"), arg("valueType") = Ts_GetType<double>()))

        .def("GetSplineByName", &This::GetSplineByName,
            (arg("name"), arg("valueType") = Ts_GetType<double>()))
        ;
}
