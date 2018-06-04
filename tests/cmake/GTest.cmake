find_package(Threads REQUIRED)

target_link_libraries(tests PUBLIC Threads::Threads)


include(ExternalProject)

ExternalProject_Add(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(googletest source_dir)
ExternalProject_Get_Property(googletest binary_dir)


add_dependencies(tests googletest)

target_include_directories(tests PUBLIC ${source_dir}/googletest/include ${binary_dir}/googlemock/gtest)

target_link_libraries(tests PUBLIC ${binary_dir}/googlemock/gtest/libgtest.a ${binary_dir}/googlemock/gtest/libgtest_main.a)
