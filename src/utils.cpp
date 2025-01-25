/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

#include "utils.hpp"

/// \cond
#include <exception>
#include <iostream>

/// \endcond

/*****************************************************************************/
/*** FREE FUNCTIONS **********************************************************/

void annotate_and_terminate(const char* message, const char* file, int line)
{
    std::cout << "Program panicked with\n"
              << "  " << message << '\n'
              << "On File: " << file << '\n'
              << "At Line: " << line << '\n';

    std::terminate();
}
