find_package(Doxygen OPTIONAL_COMPONENTS dot)

if(NOT Doxygen_FOUND)
    message(STATUS "Doxygen not found! Documentation disabled.")
    return()
endif()

set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/docs)

set(DOXYGEN_RECURSIVE YES)
set(DOXYGEN_EXCLUDE_PATTERNS */build/* */tests/* */vcpkg_installed/*)

set(DOXYGEN_GENERATE_LATEX NO)
set(DOXYGEN_GENERATE_TREEVIEW YES)

set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_EXTRACT_PRIVATE YES)
set(DOXYGEN_EXTRACT_STATIC YES)

if(TARGET Doxygen::dot)
    set(DOXYGEN_HAVE_DOT YES)
else()
    set(DOXYGEN_HAVE_DOT NO)
    message(STATUS "Graphviz 'dot' not found! Documentation graphs disabled.")
endif()

set(DOXYGEN_CALL_GRAPH ${DOXYGEN_HAVE_DOT})
set(DOXYGEN_CALLER_GRAPH ${DOXYGEN_HAVE_DOT})

doxygen_add_docs(docs
    WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}
    COMMENT
        "Generating project documentation"
)
