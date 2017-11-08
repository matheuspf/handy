get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)

include_directories(${GTEST_INCLUDE_DIRS} ${PARENT_DIR}/include)

file(GLOB ALGORITHMS_SRC_FILES ${PROJECT_SOURCE_DIR}/Algorithms/*.cpp)

set(TEST_SRC_FILES ${TEST_SRC_FILES} ${ALGORITHMS_SRC_FILES})


set(ALGORITHMS_TEST_CPP ${PROJECT_SOURCE_DIR}/Algorithms/AlgorithmsTest.cpp)

set_source_files_properties(${ALGORITHMS_TEST_CPP} PROPERTIES COMPILE_FLAGS "-std=c++17")