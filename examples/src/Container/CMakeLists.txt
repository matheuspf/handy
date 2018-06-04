include(${PROJECT_SOURCE_DIR}/examples/cmake/AddExample.cmake)

set(container_files Container.cpp Slice.cpp)

addExample(${CMAKE_CURRENT_SOURCE_DIR} ${container_files})