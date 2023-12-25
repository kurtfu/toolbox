#ifndef RESULT_HPP
#define RESULT_HPP
// NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)

/*****************************************************************************/
/*  HEADER INCLUDES                                                          */
/*****************************************************************************/

#include <memory>

/*****************************************************************************/
/*  MACRO DEFINITIONS                                                        */
/*****************************************************************************/

#if __cplusplus < 201703L
    #define SUCCESS_NODISCARD
    #define FAIL_NODISCARD
    #define RESULT_NODISCARD
#else
    #define SUCCESS_NODISCARD [[nodiscard]]
    #define FAIL_NODISCARD    [[nodiscard]]
    #define RESULT_NODISCARD  [[nodiscard]]
#endif  // __cplusplus

#if __cplusplus > 201703L
    #define RESULT_CONSTEXPR_DESTRUCTOR constexpr
#else
    #define RESULT_CONSTEXPR_DESTRUCTOR
#endif  // __cplusplus

/*****************************************************************************/
/*  DATA TYPES                                                               */
/*****************************************************************************/

template <typename T>
class success
{
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;

    template <typename... Args>
    explicit success(Args... args)
        : m_storage(std::forward<Args>(args)...)
    {}

    SUCCESS_NODISCARD
    reference value()
    {
        return m_storage;
    }

    SUCCESS_NODISCARD
    const_reference value() const
    {
        return m_storage;
    }

private:
    value_type m_storage;
};

template <typename T>
class fail
{
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;

    template <typename... Args>
    explicit fail(Args... args)
        : m_storage(std::forward<Args>(args)...)
    {}

    FAIL_NODISCARD
    reference value()
    {
        return m_storage;
    }

    FAIL_NODISCARD
    const_reference value() const
    {
        return m_storage;
    }

private:
    value_type m_storage;
};

template <typename T, typename E>
class result
{
public:
    using value_type = T;
    using error_type = E;

    // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
    constexpr result(success<value_type>&& item)
        : m_has_value(true)
    {
        ::new (std::addressof(m_value)) value_type(std::move(item.value()));
        std::ignore = std::move(item);
    }

    // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
    constexpr result(fail<error_type>&& item)
        : m_has_value(false)
    {
        ::new (std::addressof(m_error)) error_type(std::move(item.value()));
        std::ignore = std::move(item);
    }

    constexpr result(const result& that)
        : m_has_value(that.m_has_value)
    {
        if (m_has_value)
        {
            ::new (std::addressof(m_value)) value_type(that.m_value);
        }
        else
        {
            ::new (std::addressof(m_error)) error_type(that.m_error);
        }
    }

    constexpr result(result&& that) noexcept
        : m_has_value(that.m_has_value)
    {
        if (m_has_value)
        {
            ::new (std::addressof(m_value)) value_type(std::move(that.m_value));
        }
        else
        {
            ::new (std::addressof(m_error)) error_type(std::move(that.m_error));
        }
    }

    RESULT_CONSTEXPR_DESTRUCTOR
    ~result()
    {
        if (m_has_value)
        {
            m_value.~value_type();
        }
        else
        {
            m_error.~error_type();
        }
    }

    result& operator=(const result& that)
    {
        if (this != &that)
        {
            m_has_value = that.m_has_value;

            if (m_has_value)
            {
                ::new (std::addressof(m_value)) value_type(that.m_value);
            }
            else
            {
                ::new (std::addressof(m_error)) error_type(that.m_error);
            }
        }

        return *this;
    }

    result& operator=(result&& that) noexcept
    {
        if (this != &that)
        {
            m_has_value = that.m_has_value;

            if (m_has_value)
            {
                ::new (std::addressof(m_value)) value_type(std::move(that.m_value));
            }
            else
            {
                ::new (std::addressof(m_error)) error_type(std::move(that.m_error));
            }
        }

        return *this;
    }

    RESULT_NODISCARD
    bool has_value() const
    {
        return m_has_value;
    }

    explicit operator bool() const noexcept
    {
        return m_has_value;
    }

private:
    union
    {
        value_type m_value;
        error_type m_error;
    };

    bool m_has_value;
};

#undef SUCCESS_NODISCARD
#undef FAIL_NODISCARD
#undef RESULT_NODISCARD

#undef RESULT_CONSTEXPR_DESTRUCTOR

// NOLINTEND(cppcoreguidelines-pro-type-union-access)
#endif  // RESULT_HPP
