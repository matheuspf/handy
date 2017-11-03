cmake_minimum_required(VERSION 2.8)

set(TEST_NAME CPPL_Tests)

project (${TEST_NAME})


find_package(Threads REQUIRED)

add_subdirectory(gtest)

enable_testing()


set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -g -fprofile-arcs -ftest-coverage -Wno-deprecated -pthread -fmax-errors=2")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

include_directories(${GTEST_INCLUDE_DIRS})


FILE(GLOB children RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/*)

FOREACH(child ${children})
  IF(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${child} AND
     EXISTS ${PROJECT_SOURCE_DIR}/${child}/CMakeLists.txt AND
     NOT ${child} STREQUAL "build" AND 
     NOT ${child} STREQUAL "gtest" AND
     NOT ${child} STREQUAL "Algorithms")

     include(${child}/CMakeLists.txt)

  ENDIF()
ENDFOREACH()


add_executable(${TEST_NAME} ${TEST_SRC_FILES})

add_dependencies(${TEST_NAME} googletest)


target_link_libraries(${TEST_NAME} ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a)

target_link_libraries(${TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})

add_test(test1 ${TEST_NAME})