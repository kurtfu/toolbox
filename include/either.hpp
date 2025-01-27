#ifndef EITHER_HPP
#define EITHER_HPP

/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

/// \cond
#include <memory>
#include <utility>

/// \endcond

/*****************************************************************************/
/*** CLASSES *****************************************************************/

template <typename Left, typename Right = void>
class either_t
{
    struct left_t
    {};

    struct right_t
    {};

    struct empty_t
    {};

public:
    static constexpr left_t left;
    static constexpr right_t right;

    constexpr either_t() = default;

    template <typename... Args>
    explicit constexpr either_t(left_t /* unused */, Args&&... args)
        : m_storage(left, std::forward<Args>(args)...)
    {}

    template <typename... Args>
    explicit constexpr either_t(right_t /* unused */, Args&&... args)
        : m_storage(right, std::forward<Args>(args)...)
    {}

    template <typename... Args>
    void construct(left_t /* unused */, Args&&... args)
    {
        ::new (std::addressof(this->get(left))) Left(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void construct(right_t /* unused */, Args&&... args)
    {
        ::new (std::addressof(this->get(right))) Right(std::forward<Args>(args)...);
    }

    void destruct(left_t /* unused */)
    {
        this->get(left).~Left();
    }

    void destruct(right_t /* unused */)
    {
        this->get(right).~Right();
    }

    constexpr Left& get(left_t /* unused */) noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        return m_storage.m_left;
    }

    [[nodiscard]] constexpr const Left& get(left_t /* unused */) const noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        return m_storage.m_left;
    }

    constexpr Right& get(right_t /* unused */) noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        return m_storage.m_right;
    }

    [[nodiscard]] constexpr const Right& get(right_t /* unused */) const noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        return m_storage.m_right;
    }

private:
    union storage_t
    {
        constexpr storage_t()
            : m_nothing()
        {}

        template <typename... Args>
        explicit constexpr storage_t(left_t /* unused */, Args&&... args)
            : m_left(std::forward<Args>(args)...)
        {}

        template <typename... Args>
        explicit constexpr storage_t(right_t /* unused */, Args&&... args)
            : m_right(std::forward<Args>(args)...)
        {}

        constexpr storage_t(const storage_t&) = delete;
        constexpr storage_t(storage_t&&) noexcept = delete;

        ~storage_t() {};

        constexpr storage_t& operator=(const storage_t&) = delete;
        constexpr storage_t& operator=(storage_t&&) noexcept = delete;

        Left m_left;
        Right m_right;

        empty_t m_nothing;
    };

    storage_t m_storage;
};

template <typename Left>
class either_t<Left, void>
{
    struct left_t
    {};

    struct right_t
    {};

    struct empty_t
    {};

public:
    static constexpr left_t left;
    static constexpr right_t right;

    constexpr either_t() = default;

    template <typename... Args>
    explicit constexpr either_t(left_t /* unused */, Args&&... args)
        : m_storage(left, std::forward<Args>(args)...)
    {}

    template <typename... Args>
    void construct(left_t /* unused */, Args&&... args)
    {
        ::new (std::addressof(this->get(left))) Left(std::forward<Args>(args)...);
    }

    void destruct(left_t /* unused */)
    {
        this->get(left).~Left();
    }

    constexpr Left& get(left_t /* unused */) noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        return m_storage.m_left;
    }

    [[nodiscard]] constexpr const Left& get(left_t /* unused */) const noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        return m_storage.m_left;
    }

private:
    union storage_t
    {
        constexpr storage_t()
            : m_nothing()
        {}

        template <typename... Args>
        explicit constexpr storage_t(left_t /* unused */, Args&&... args)
            : m_left(std::forward<Args>(args)...)
        {}

        constexpr storage_t(const storage_t&) = delete;
        constexpr storage_t(storage_t&&) noexcept = delete;

        ~storage_t() {};

        constexpr storage_t& operator=(const storage_t&) = delete;
        constexpr storage_t& operator=(storage_t&&) noexcept = delete;

        Left m_left;
        empty_t m_nothing;
    };

    storage_t m_storage;
};

#endif  // EITHER_HPP
