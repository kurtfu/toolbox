/*****************************************************************************/
/*  HEADER INCLUDES                                                          */
/*****************************************************************************/

#include <catch2/catch_test_macros.hpp>

#include "either.hpp"

#include <string>

/*****************************************************************************/
/*  TEST CASES                                                               */
/*****************************************************************************/

utils::either<int, std::string> good()
{
    return utils::success<int>(0);
}

utils::either<int, std::string> bad()
{
    return utils::fail<std::string>("bad");
}

TEST_CASE("Simple success assignment")
{
    auto ret = good();

    REQUIRE(ret.has_value() == true);
    REQUIRE_NOTHROW(ret.value() == 0);
}

TEST_CASE("Simple fail assignment")
{
    auto ret = bad();

    REQUIRE(ret.has_value() == false);
    REQUIRE_NOTHROW(ret.error() == "bad");
}

TEST_CASE("Bad value access")
{
    auto ret = bad();

    REQUIRE_THROWS_AS(ret.value(), utils::bad_value_exception);
}

TEST_CASE("Bad error access")
{
    auto ret = good();

    REQUIRE_THROWS_AS(ret.error(), utils::bad_value_exception);
}
