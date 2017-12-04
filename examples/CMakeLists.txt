cmake_minimum_required(VERSION 2.8)

set(EXAMPLE_NAME HandyExample)

project(${EXAMPLE_NAME})


get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)

include_directories(${PARENT_DIR}/include)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")


FILE(GLOB children RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/*)

FOREACH(child ${children})
  IF(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${child} AND
     EXISTS ${PROJECT_SOURCE_DIR}/${child}/CMakeLists.txt AND
     NOT ${child} STREQUAL "build")

     include(${child}/CMakeLists.txt)

  ENDIF()
ENDFOREACH()


FOREACH(FILE_CPP ${EXAMPLE_SRC_FILES})
  get_filename_component(FILE ${FILE_CPP} NAME_WE)
  add_executable(${FILE} ${FILE_CPP})
ENDFOREACH()