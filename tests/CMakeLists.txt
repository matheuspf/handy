cmake_minimum_required(VERSION 2.8)

set(TEST_NAME CPPL_Tests)

project (${TEST_NAME})


find_package(Threads REQUIRED)

add_subdirectory(gtest)

enable_testing()

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -g -fprofile-arcs -ftest-coverage -Wno-deprecated -pthread")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

include_directories(${GTEST_INCLUDE_DIRS})


add_subdirectory(Container)
add_subdirectory(ZipIter)

set(TEST_SRC_FILES ${CONTAINER_SRC_FILES} ${ZIP_ITER_SRC_FILES})

add_executable(${TEST_NAME} ${TEST_SRC_FILES})

add_dependencies(${TEST_NAME} googletest)


target_link_libraries(${TEST_NAME} ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a)

target_link_libraries(${TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})

add_test(test1 ${TEST_NAME})