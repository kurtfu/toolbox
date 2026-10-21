#ifndef RESULT_HPP
#define RESULT_HPP

/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

#include "either.hpp"

/// \cond
#include <type_traits>
#include <utility>

/// \endcond

/*****************************************************************************/
/*** MACRO DEFINITIONS *******************************************************/

#if __cplusplus > 201703L
    #define RESULT_CONSTEXPR_DESTRUCTOR constexpr
#else
    #define RESULT_CONSTEXPR_DESTRUCTOR
#endif  // __cplusplus

/*****************************************************************************/
/*** CLASSES *****************************************************************/

template <typename T>
class success_t
{
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;

    template <typename... Args>
    explicit success_t(Args... args)
        : m_storage(std::forward<Args>(args)...)
    {}

    constexpr reference value()
    {
        return m_storage;
    }

    [[nodiscard]] constexpr const_reference value() const
    {
        return m_storage;
    }

    constexpr reference operator*()
    {
        return m_storage;
    }

    [[nodiscard]] constexpr const_reference operator*() const
    {
        return m_storage;
    }

private:
    value_type m_storage;
};

template <typename T>
class fail_t
{
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;

    template <typename... Args>
    explicit fail_t(Args... args)
        : m_storage(std::forward<Args>(args)...)
    {}

    constexpr reference value()
    {
        return m_storage;
    }

    [[nodiscard]] constexpr const_reference value() const
    {
        return m_storage;
    }

    constexpr reference operator*()
    {
        return m_storage;
    }

    [[nodiscard]] constexpr const_reference operator*() const
    {
        return m_storage;
    }

private:
    value_type m_storage;
};

template <typename Value, typename Error, typename = void>
class result_t;

template <typename Value, typename Error>
class result_t<Value, Error, std::enable_if_t<!std::is_void_v<Value>>>
{
    using storage_type = either_t<Value, Error>;

    static constexpr auto value = storage_type::left;
    static constexpr auto error = storage_type::right;

public:
    using value_type = Value;
    using error_type = Error;

    // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
    constexpr result_t(success_t<value_type> item)
        : m_storage(value, std::move(*item))
        , m_has_value(true)
    {}

    // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
    constexpr result_t(fail_t<error_type> item)
        : m_storage(error, std::move(*item))
        , m_has_value(false)
    {}

    constexpr result_t(const result_t& that) = delete;
    constexpr result_t(result_t&& that) = delete;

    RESULT_CONSTEXPR_DESTRUCTOR
    ~result_t()
    {
        if (m_has_value)
        {
            m_storage.destruct(value);
        }
        else
        {
            m_storage.destruct(error);
        }
    }

    result_t& operator=(const result_t& that) = delete;
    result_t& operator=(result_t&& that) = delete;

    [[nodiscard]] constexpr bool has_value() const
    {
        return m_has_value;
    }

    explicit operator bool() const noexcept
    {
        return m_has_value;
    }

private:
    storage_type m_storage;
    bool m_has_value;
};

template <typename Value, typename Error>
class result_t<Value, Error, std::enable_if_t<std::is_void_v<Value>>>
{
    using storage_type = either_t<Error>;

    static constexpr auto error = storage_type::left;

public:
    using value_type = Value;
    using error_type = Error;

    // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
    constexpr result_t()
        : m_has_value(true)
    {}

    // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
    constexpr result_t(success_t<value_type> /* item */)
        : m_has_value(true)
    {}

    // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
    constexpr result_t(fail_t<error_type> item)
        : m_storage(error, std::move(*item))
        , m_has_value(false)
    {}

    constexpr result_t(const result_t& that) = delete;
    constexpr result_t(result_t&& that) = delete;

    RESULT_CONSTEXPR_DESTRUCTOR
    ~result_t()
    {
        if (!m_has_value)
        {
            m_storage.destruct(error);
        }
    }

    result_t& operator=(const result_t& that) = delete;
    result_t& operator=(result_t&& that) = delete;

    [[nodiscard]] constexpr bool has_value() const
    {
        return m_has_value;
    }

    explicit operator bool() const noexcept
    {
        return m_has_value;
    }

private:
    storage_type m_storage;
    bool m_has_value;
};

template <typename T>
success_t(T) -> success_t<T>;

template <typename T>
fail_t(T) -> fail_t<T>;

#undef RESULT_CONSTEXPR_DESTRUCTOR

#endif  // RESULT_HPP
