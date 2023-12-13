find_package(Doxygen)

if(NOT Doxygen_FOUND)
    message(STATUS "Doxygen not found! Documentation disabled.")
    return()
endif()

set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/docs)

set(DOXYGEN_RECURSIVE YES)
set(DOXYGEN_EXCLUDE_PATTERNS */build/* */tests/*)

set(DOXYGEN_HAVE_DOT YES)
set(DOXYGEN_GENERATE_LATEX NO)

set(DOXYGEN_GENERATE_TREEVIEW YES)

set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_EXTRACT_PRIVATE YES)
set(DOXYGEN_EXTRACT_STATIC YES)

set(DOXYGEN_CALL_GRAPH YES)
set(DOXYGEN_CALLER_GRAPH YES)

doxygen_add_docs(docs
    WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}
    COMMENT
        "Generating project documentation"
)
