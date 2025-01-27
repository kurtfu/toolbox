/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

#include <catch2/catch_test_macros.hpp>

#include "either.hpp"

/// \cond
#include <string>

/// \endcond

/*****************************************************************************/
/*** TEST CASES **************************************************************/

using test_type = either_t<int, std::string>;

static either_t<int, std::string> good()
{
    return test_type(test_type::left, 1);
}

static either_t<int, std::string> bad()
{
    return test_type(test_type::right, "bad");
}

TEST_CASE("Simple success assignment")
{
    auto result = good();
    auto value = result.get(test_type::left);

    REQUIRE_NOTHROW(value == 1);
}

TEST_CASE("Simple fail assignment")
{
    auto result = bad();
    const auto& value = result.get(test_type::right);

    REQUIRE_NOTHROW(value == "bad");
}
