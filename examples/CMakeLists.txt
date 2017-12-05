cmake_minimum_required(VERSION 2.8)

set(EXAMPLE_NAME HandyExample)

project(${EXAMPLE_NAME})


get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)

include_directories(${PARENT_DIR}/include)



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



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wno-unused-value -Wno-deprecated")



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
  set(FILES ${FILES} ${FILE})

ENDFOREACH()



add_custom_command( OUTPUT HandyExample.out
                    COMMAND ${CMAKE_COMMAND} ${PARENT_DIR}/examples/Range/RangeExample
                    DEPENDS ${PARENT_DIR}/examples/Range/RangeExample
                  )

add_custom_target(HandyExample DEPENDS HandyExample.out)