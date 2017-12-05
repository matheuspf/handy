cmake_minimum_required(VERSION 2.8)

set(TEST_NAME HandyTest)

project (${TEST_NAME})

find_package(Threads REQUIRED)

find_package(GTest)


if(NOT ${GTEST_FOUND})
  add_subdirectory(gtest)
endif()

enable_testing()


include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)

if(COMPILER_SUPPORTS_CXX17)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

elseif(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

elseif(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

else()
  message( FATAL_ERROR "Handy cannot be compiled on ${CMAKE_CXX_COMPILER} because it does not support C++11")

endif()



set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage -Wno-deprecated -pthread -fmax-errors=2")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

include_directories(${GTEST_INCLUDE_DIRS})


FILE(GLOB children RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/*)

FOREACH(child ${children})
  IF(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${child} AND
     EXISTS ${PROJECT_SOURCE_DIR}/${child}/CMakeLists.txt AND
     NOT ${child} STREQUAL "build" AND 
     NOT ${child} STREQUAL "gtest")

     include(${child}/CMakeLists.txt)

  ENDIF()
ENDFOREACH()



add_executable(${TEST_NAME} ${TEST_SRC_FILES})


if(NOT ${GTEST_FOUND})

  add_dependencies(${TEST_NAME} googletest)
  
  set(GTEST_LIBRARIES ${GTEST_LIBS_DIR}/libgtest.a)
  set(GTEST_MAIN_LIBRARIES ${GTEST_LIBS_DIR}/libgtest_main.a)

endif()


target_link_libraries(${TEST_NAME} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

target_link_libraries(${TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})


add_test(test1 ${TEST_NAME})