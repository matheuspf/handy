add_executable(tests)

target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_features(tests PUBLIC cxx_std_17)

target_compile_options(tests PUBLIC -g -O0 --coverage -fprofile-arcs -ftest-coverage)

target_link_libraries(tests PUBLIC -lgcov)



add_subdirectory(src/Algorithms)
add_subdirectory(src/Container)



enable_testing()

find_package(GTest)

if(TARGET GTest::GTest)

   target_link_libraries(tests PUBLIC GTest::GTest GTest::Main)

else()

    include(gtest/CMakeLists.txt)

endif()


add_test(allTests tests)