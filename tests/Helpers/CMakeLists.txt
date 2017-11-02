get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)

include_directories(${GTEST_INCLUDE_DIRS} ${PARENT_DIR}/include)

file(GLOB Helpers_SRC_FILES ${PROJECT_SOURCE_DIR}/Helpers/*.cpp)

set(TEST_SRC_FILES ${TEST_SRC_FILES} ${Helpers_SRC_FILES})