include(${PROJECT_SOURCE_DIR}/examples/cmake/AddExample.cmake)

set(algorithms_files Algorithms.cpp)

addExample(${CMAKE_CURRENT_SOURCE_DIR} ${algorithms_files})