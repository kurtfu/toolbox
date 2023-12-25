/*****************************************************************************/
/*  HEADER INCLUDES                                                          */
/*****************************************************************************/

#include <catch2/catch_test_macros.hpp>

#include "result.hpp"

#include <string>

/*****************************************************************************/
/*  TEST CASES                                                               */
/*****************************************************************************/

result<int, std::string> good()
{
    return success<int>{};
}

result<int, std::string> bad()
{
    return fail<std::string>{};
}

TEST_CASE("Success")
{
    auto ret = good();

    REQUIRE(ret.has_value() == true);
}

TEST_CASE("Fail")
{
    auto ret = bad();

    REQUIRE(ret.has_value() == false);
}
