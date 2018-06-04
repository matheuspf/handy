include(${PROJECT_SOURCE_DIR}/examples/cmake/AddExample.cmake)

set(helper_files Benchmark.cpp HandyParams.cpp HasMember.coo NamedTuple.cpp Print.cpp Random.cpp)

addExample(${CMAKE_CURRENT_SOURCE_DIR} ${helper_files})