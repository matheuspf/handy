file(GLOB Helpers_SRC_FILES ${PROJECT_SOURCE_DIR}/Helpers/*.cpp)

set(EXAMPLE_SRC_FILES ${EXAMPLE_SRC_FILES} ${Helpers_SRC_FILES})


set(HANDY_PARAMS_SRC ${PROJECT_SOURCE_DIR}/Helpers/HandyParamsExample.cpp)

set_source_files_properties(${HANDY_PARAMS_SRC} PROPERTIES COMPILE_FLAGS "-std=c++17")