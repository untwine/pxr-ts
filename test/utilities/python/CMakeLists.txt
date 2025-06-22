add_library(pyTsTest SHARED
    module.cpp
    wrapMuseum.cpp
    wrapSampleBezier.cpp
    wrapSampleTimes.cpp
    wrapSplineData.cpp
    wrapTsEvaluator.cpp
    wrapTypes.cpp
)

if (PXR_BUILD_ANIMX_TESTS)
    target_sources(pyTsTest
        PRIVATE wrapAnimXEvaluator.cpp)
endif()

set_target_properties(pyTsTest
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME _ts_test
        RUNTIME_OUTPUT_DIRECTORY "$<CONFIG>/ts_test"
        LIBRARY_OUTPUT_DIRECTORY "$<CONFIG>/ts_test"
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(pyTsTest
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(pyTsTest
    PUBLIC
        tsTest
        pxr::boost-python
)

if(WIN32)
    set_target_properties(pyTsTest PROPERTIES SUFFIX ".pyd")
elseif(APPLE)
    set_target_properties(pyTsTest PROPERTIES SUFFIX ".so")
endif()

target_compile_definitions(
    pyTsTest
    PRIVATE
        BOOST_BIND_GLOBAL_PLACEHOLDERS
        BOOST_PYTHON_NO_PY_SIGNATURES
        MFB_PACKAGE_MODULE=TsTest
        MFB_PACKAGE_NAME=ts_test
        MFB_ALT_PACKAGE_NAME=ts_test
)

file(
    GENERATE
    OUTPUT "$<TARGET_FILE_DIR:pyTsTest>/__init__.py"
    INPUT "__init__.py"
)

file(
    GENERATE
    OUTPUT "$<TARGET_FILE_DIR:pyTsTest>/TsTest_Baseliner.py"
    INPUT "TsTest_Baseliner.py"
)

file(
    GENERATE
    OUTPUT "$<TARGET_FILE_DIR:pyTsTest>/TsTest_Comparator.py"
    INPUT "TsTest_Comparator.py"
)

file(
    GENERATE
    OUTPUT "$<TARGET_FILE_DIR:pyTsTest>/TsTest_Grapher.py"
    INPUT "TsTest_Grapher.py"
)

if (PXR_BUILD_MAYAPY_TESTS)
    file(
        GENERATE
        OUTPUT "$<TARGET_FILE_DIR:pyTsTest>/TsTest_MayapyDriver.py"
        INPUT "TsTest_MayapyDriver.py"
    )

    file(
        GENERATE
        OUTPUT "$<TARGET_FILE_DIR:pyTsTest>/TsTest_MayapyEvaluator.py"
        INPUT "TsTest_MayapyEvaluator.py"
    )
endif()
