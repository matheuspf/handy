include(${PROJECT_SOURCE_DIR}/examples/cmake/AddExample.cmake)

set(zip_iter_files Looping.cpp STL.cpp ZipIter.cpp)

addExample(${CMAKE_CURRENT_SOURCE_DIR} ${zip_iter_files})