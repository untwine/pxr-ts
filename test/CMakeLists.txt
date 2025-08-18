add_subdirectory(utilities)

set(data_src "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(_env "")

if (WIN32)
    # Patch CMake's 'add_test' to extend the DLL search path on Windows. Use the
    # target name associated with the test, if it is also a CMake target.

    macro(add_test)
        _add_test(${ARGV})

        cmake_parse_arguments("" "" "NAME" "" ${ARGV})
        if (TARGET ${_NAME})
            set(DLL_DIRS $<TARGET_RUNTIME_DLL_DIRS:${_NAME}>)
        else()
            set(DLL_DIRS $<TARGET_RUNTIME_DLL_DIRS:pyTs>)
        endif()
        set(DLL_DIRS
            ${DLL_DIRS}
            "${onetbb_LIB_DIRS_RELEASE}"
            "${onetbb_BIN_DIRS_RELEASE}"
        )

        set(_env "PATH=$<JOIN:$<SHELL_PATH:${DLL_DIRS}>,\\;>")
        set_tests_properties(${_NAME} PROPERTIES ENVIRONMENT "${_env}")
    endmacro()
endif()

add_executable(testTsThreadedCOW testTsThreadedCOW.cpp)
target_link_libraries(testTsThreadedCOW PUBLIC ts pxr::tf)
add_test(NAME testTsThreadedCOW COMMAND testTsThreadedCOW)

add_executable(testTsSplineAPI testTsSplineAPI.cpp)
target_link_libraries(testTsSplineAPI PUBLIC ts pxr::tf pxr::vt)
add_test(NAME testTsSplineAPI COMMAND testTsSplineAPI)

add_executable(testTsSplineSampling testTsSplineSampling.cpp)
target_link_libraries(testTsSplineSampling PUBLIC ts pxr::tf pxr::vt tsTest)
add_test(NAME testTsSplineSampling COMMAND testTsSplineSampling)

add_executable(testTsTangentConversion testTsTangentConversion.cpp)
target_link_libraries(testTsTangentConversion PUBLIC ts pxr::tf pxr::vt)
add_test(NAME testTsTangentConversion COMMAND testTsTangentConversion)

add_executable(testTsSplineEval testTsSplineEval.cpp)
target_link_libraries(testTsSplineEval PUBLIC ts pxr::tf pxr::vt)
add_test(NAME testTsSplineEval COMMAND testTsSplineEval)

add_executable(testTsRegressionPreventerCpp testTsRegressionPreventerCpp.cpp)
target_link_libraries(testTsRegressionPreventerCpp PUBLIC ts pxr::tf)
add_test(NAME testTsRegressionPreventerCpp COMMAND testTsRegressionPreventerCpp)

add_executable(testTsSegmentIterator testTsSegmentIterator.cpp)
target_link_libraries(testTsSegmentIterator PUBLIC ts pxr::tf)
add_test(NAME testTsSegmentIterator COMMAND testTsSegmentIterator)

add_executable(testTsSplineBaking testTsSplineBaking.cpp)
target_link_libraries(testTsSplineBaking PUBLIC ts pxr::tf tsTest)
add_test(NAME testTsSplineBaking COMMAND testTsSplineBaking)

add_executable(testTsSplineDiff testTsSplineDiff.cpp)
target_link_libraries(testTsSplineDiff PUBLIC ts pxr::tf)
add_test(NAME testTsSplineDiff COMMAND testTsSplineDiff)

add_executable(testTsSplineTransforms testTsSplineTransforms.cpp)
target_link_libraries(testTsSplineTransforms PUBLIC ts pxr::tf tsTest)
add_test(NAME testTsSplineTransforms COMMAND testTsSplineTransforms)

if(BUILD_PYTHON_BINDINGS)
    pytest_discover_tests(
        TestPyTs
        LIBRARY_PATH_PREPEND
            $<TARGET_FILE_DIR:pyTs>
            $<TARGET_FILE_DIR:pyTsTest>
            "${onetbb_LIB_DIRS_RELEASE}"
            "${onetbb_BIN_DIRS_RELEASE}"
        PYTHON_PATH_PREPEND
            "$<TARGET_FILE_DIR:pyTs>/../.."
            "$<TARGET_FILE_DIR:pyTsTest>/.."
            "$<TARGET_FILE_DIR:pxr::pyVt>/../.."
            "$<TARGET_FILE_DIR:pxr::pyGf>/../.."
            "$<TARGET_FILE_DIR:pxr::pyTf>/../.."
        TRIM_FROM_NAME "^test_"
        TRIM_FROM_FULL_NAME "^(TsTest_|TestTs)"
        ENVIRONMENT
            "TS_REGRESSION_PREVENTER_PATH=$<SHELL_PATH:${data_src}/tsRegressionPreventer>"
            "TS_FRAMEWORK_PATH=$<SHELL_PATH:${data_src}/tsFramework>"
        EXTRA_ARGS "-v"
        DEPENDS ts pyTs tsTest pyTsTest
    )
endif()