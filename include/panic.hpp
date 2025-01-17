#ifndef PANIC_HPP
#define PANIC_HPP

/*****************************************************************************/
/*** MACRO DEFINITIONS *******************************************************/

#define panic(message) annotate_and_terminate(message, __FILE__, __LINE__)

/*****************************************************************************/
/*** FUNCTION DEFINITIONS ****************************************************/

void annotate_and_terminate(const char* message, const char* file, int line);

#endif  // PANIC_HPP
