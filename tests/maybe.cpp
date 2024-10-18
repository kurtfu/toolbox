/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

#include <catch2/catch_test_macros.hpp>

#include "maybe.hpp"

/// \cond
#include <cstddef>
#include <memory>
#include <utility>

/// \endcond

/*****************************************************************************/
/*** DATA TYPES **************************************************************/

struct Counters
{
    std::size_t constructor_call;

    std::size_t copy_constructor_call;
    std::size_t move_constructor_call;

    std::size_t destructor_call;

    std::size_t copy_assignment_call;
    std::size_t move_assignment_call;

    std::size_t swap_call;
};

struct Widget
{
    Counters* m_counters = nullptr;

    explicit Widget(Counters* counters)
        : m_counters(counters)
    {
        m_counters->constructor_call += 1;
    }

    Widget(const Widget& that)
        : m_counters(that.m_counters)
    {
        m_counters->copy_constructor_call += 1;
    }

    Widget(Widget&& that) noexcept
        : m_counters(that.m_counters)
    {
        m_counters->move_constructor_call += 1;
    }

    ~Widget()
    {
        m_counters->destructor_call += 1;
    }

    Widget& operator=(const Widget& that)
    {
        if (this == std::addressof(that))
        {
            return *this;
        }

        this->m_counters = that.m_counters;
        this->m_counters->copy_assignment_call += 1;

        return *this;
    }

    Widget& operator=(Widget&& that) noexcept
    {
        this->m_counters = that.m_counters;
        this->m_counters->move_assignment_call += 1;

        return *this;
    }

    constexpr void swap(Widget& /* unused */) const noexcept
    {
        m_counters->swap_call += 1;
    }

    constexpr friend void swap(Widget& lhs, Widget& rhs) noexcept
    {
        lhs.swap(rhs);
    }
};

/*****************************************************************************/
/*** TEST CASES **************************************************************/

TEST_CASE("Call Me Maybe")
{
    Counters counters{};

    {
        utils::maybe<Widget> widget{std::addressof(counters)};
        utils::maybe<Widget> none = utils::nothing;

        auto copy = widget;
        auto move = std::move(widget);

        REQUIRE(!none.has_value());
        REQUIRE(copy.has_value());
        REQUIRE(move.has_value());
    }

    REQUIRE(counters.constructor_call == 1);

    REQUIRE(counters.copy_constructor_call == 1);
    REQUIRE(counters.move_constructor_call == 1);

    REQUIRE(counters.destructor_call == 2);

    REQUIRE(counters.copy_assignment_call == 0);
    REQUIRE(counters.move_assignment_call == 0);

    REQUIRE(counters.swap_call == 0);
}

TEST_CASE("Assign Me Maybe")
{
    Counters counters{};

    {
        utils::maybe<Widget> widget{std::addressof(counters)};

        utils::maybe<Widget> copy = utils::nothing;
        utils::maybe<Widget> move = utils::nothing;

        utils::maybe<Widget> none = utils::nothing;

        copy = widget;
        move = std::move(widget);

        auto* shadow = std::addressof(copy);
        copy = *shadow;

        copy = move;
        move = std::move(copy);

        copy = none;
        move = utils::nothing;

        REQUIRE(!copy.has_value());
        REQUIRE(!move.has_value());
    }

    REQUIRE(counters.constructor_call == 1);

    REQUIRE(counters.copy_constructor_call == 1);
    REQUIRE(counters.move_constructor_call == 1);

    REQUIRE(counters.destructor_call == 2);

    REQUIRE(counters.copy_assignment_call == 1);
    REQUIRE(counters.move_assignment_call == 1);

    REQUIRE(counters.swap_call == 0);
}

TEST_CASE("Swap Me Maybe")
{
    Counters counters{};

    {
        utils::maybe<Widget> lhs{std::addressof(counters)};
        utils::maybe<Widget> rhs{std::addressof(counters)};

        utils::maybe<Widget> none = utils::nothing;

        swap(lhs, rhs);

        lhs.swap(none);
        lhs.swap(rhs);

        REQUIRE(lhs.has_value());
        REQUIRE(!rhs.has_value());
    }

    REQUIRE(counters.constructor_call == 2);

    REQUIRE(counters.copy_constructor_call == 0);
    REQUIRE(counters.move_constructor_call == 2);

    REQUIRE(counters.destructor_call == 2);

    REQUIRE(counters.copy_assignment_call == 0);
    REQUIRE(counters.move_assignment_call == 0);

    REQUIRE(counters.swap_call == 1);
}
