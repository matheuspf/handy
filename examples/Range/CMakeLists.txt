include(${PROJECT_SOURCE_DIR}/examples/cmake/AddExample.cmake)

set(range_files Range.cpp)

addExample(${CMAKE_CURRENT_SOURCE_DIR} ${range_files})