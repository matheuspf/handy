option(handy_coverage "If coverage tests will be executed" ON)


add_executable(handy_tests "")

target_include_directories(handy_tests PUBLIC ${lib_path})

target_compile_options(handy_tests PRIVATE -std=c++17)

if(handy_coverage AND  "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(handy_tests PRIVATE -g -O0 --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(handy_tests PUBLIC -lgcov)
endif()


set(handy_test_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms/Algorithms.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Container/Container.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Container/Slice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Helpers/Benchmark.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Helpers/HandyParams.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Helpers/HasMember.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Helpers/Helpers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Helpers/NamedTuple.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Helpers/Print.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Range/Range.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/Wrapper/Inheritance.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/Wrapper/Operations.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ZipIter/Looping.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/ZipIter/STL.cpp
)

target_sources(handy_tests PRIVATE ${handy_test_sources})

enable_testing()

find_package(GTest QUIET)

if(GTest_FOUND)

   target_link_libraries(handy_tests PUBLIC GTest::GTest GTest::Main)

else()

    message(STATUS "GTest not found. Downloading and installing now.")

    include(external/gtest.cmake EXCLUDE_FROM_ALL)

endif()

add_test(allTests handy_tests)