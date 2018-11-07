option(handy_coverage "If coverage tests will be executed" ON)


add_executable(tests "")

target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR}/include/handy)

target_compile_features(tests PRIVATE)

target_compile_options(tests PRIVATE -std=c++17)

if(handy_coverage AND  "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(tests PRIVATE -g -O0 --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(tests PUBLIC -lgcov)
endif()



add_subdirectory(Algorithms)
add_subdirectory(Container)
add_subdirectory(Helpers)
add_subdirectory(Range)
#add_subdirectory(Wrapper)
add_subdirectory(ZipIter)


enable_testing()

find_package(GTest QUIET)


if(GTest_FOUND)

   target_link_libraries(tests PUBLIC GTest::GTest GTest::Main)

else()

    message(STATUS "GTest not found. Downloading and installing now.")

    include(external/gtest.cmake EXCLUDE_FROM_ALL)

endif()

add_test(allTests tests)