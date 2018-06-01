cmake_minimum_required(VERSION 3.5)

project(handy VERSION 1.0.0 LANGUAGES CXX)

# add_library(handy INTERFACE)

# target_include_directories(handy INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# target_compile_options(handy INTERFACE -std=c++17 -O2)

add_subdirectory(tests)