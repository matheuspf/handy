find_package(Doxygen)

if (DOXYGEN_FOUND)

    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_GENERATE_LATEX YES)

    add_custom_target(doc ALL
       COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/doc/Doxyfile
       COMMENT "Building documentation with Doxygen")

else()
    message("You need to install Doxygen to generate the documentation")
endif()