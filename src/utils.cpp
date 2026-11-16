/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

#include "utils.hpp"

#include <exception>
#include <iostream>

/*****************************************************************************/
/*** FREE FUNCTIONS **********************************************************/

void annotate_and_terminate(std::string_view message, const char* file, int line)
{
    annotate_and_terminate(message.data(), file, line);
}

void annotate_and_terminate(const char* message, const char* file, int line)
{
    std::cerr << "Program panicked with\n"
              << "  " << message << '\n'
              << "On File: " << file << '\n'
              << "At Line: " << line << '\n';

    std::terminate();
}
