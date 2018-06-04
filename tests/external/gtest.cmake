find_package(Threads REQUIRED)

target_link_libraries(tests PUBLIC Threads::Threads)


execute_process(COMMAND git submodule update --init -- ${PROJECT_SOURCE_DIR}/tests/googletest
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/tests/external/googletest)

target_link_libraries(tests PRIVATE gtest gtest_main gmock)