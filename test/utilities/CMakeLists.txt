add_library(tsTest
    tsTest/museum.cpp
    tsTest/sampleBezier.cpp
    tsTest/sampleTimes.cpp
    tsTest/tsEvaluator.cpp
)

target_link_libraries(tsTest PUBLIC ts pxr::tf pxr::gf)

target_compile_definitions(tsTest PRIVATE TS_TEST_EXPORTS=1)

target_include_directories(tsTest
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

if(BUILD_PYTHON_BINDINGS)
    add_subdirectory(python)
endif()
