#ifndef UTILS_HPP
#define UTILS_HPP

/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

/// \cond
#include <chrono>
#include <functional>
#include <utility>

/// \endcond

/*****************************************************************************/
/*** MACRO DEFINITIONS *******************************************************/

#define panic(message) annotate_and_terminate(message, __FILE__, __LINE__)

/*****************************************************************************/
/*** CLASSES *****************************************************************/

namespace utils
{
    struct nothing_t
    {};

    struct something_t
    {};

    inline constexpr nothing_t nothing{};
    inline constexpr something_t something{};
}  // namespace utils

/*****************************************************************************/
/*** FUNCTION DEFINITIONS ****************************************************/

template <typename Duration, typename Period, typename F, typename... Args>
void repeat_for(Period period, F&& fn, Args&&... args)
{
    auto start = std::chrono::steady_clock::now();

    while (std::chrono::steady_clock::now() - start < Duration(period))
    {
        std::invoke(std::forward<F>(fn), std::forward<Args>(args)...);
    }
}

void annotate_and_terminate(const char* message, const char* file, int line);

#endif  // UTILS_HPP
