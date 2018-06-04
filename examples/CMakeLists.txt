add_executable(examples)

target_include_directories(examples PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_features(examples PRIVATE cxx_std_17)

target_compile_options(examples PRIVATE -O2)



add_subdirectory(src/Algorithms)