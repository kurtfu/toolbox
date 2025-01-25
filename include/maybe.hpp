#ifndef MAYBE_HPP
#define MAYBE_HPP

/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

#include "utils.hpp"

/// \cond
#include <functional>
#include <utility>

/// \endcond

/*****************************************************************************/
/*** CLASSES *****************************************************************/

template <typename T>
class maybe_t
{
    static_assert(!std::is_same_v<std::remove_cv_t<T>, utils::nothing_t>);
    static_assert(!std::is_same_v<std::remove_cv_t<T>, utils::something_t>);

public:
    using value_type = T;

    template <typename... Args>
        requires(std::is_constructible_v<T, Args...>)
    constexpr maybe_t(Args&&... args)  // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
        : m_storage(utils::something, std::forward<Args>(args)...)
        , m_has_value(true)
    {}

    // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
    constexpr maybe_t(utils::nothing_t /* unused */)
        : m_storage()
    {}

    constexpr maybe_t(const maybe_t& that)
    {
        if (that.has_value())
        {
            this->construct(that.get());
        }
    }

    constexpr maybe_t(maybe_t&& that) noexcept
    {
        if (that.has_value())
        {
            this->construct(std::move(that.get()));
            that.reset();
        }
    }

    constexpr ~maybe_t()
    {
        reset();
    }

    constexpr maybe_t& operator=(const maybe_t& that)
    {
        if (this == std::addressof(that))
        {
            return *this;
        }

        if (this->has_value() && that.has_value())
        {
            this->get() = that.get();
        }
        else
        {
            if (that.has_value())
            {
                this->construct(that.get());
            }
            else
            {
                this->reset();
            }
        }

        return *this;
    }

    constexpr maybe_t& operator=(maybe_t&& that) noexcept
    {
        if (this->has_value() && that.has_value())
        {
            this->get() = std::move(that.get());
        }
        else
        {
            if (that.has_value())
            {
                this->construct(std::move(that.get()));
                that.reset();
            }
            else
            {
                this->reset();
            }
        }

        return *this;
    }

    constexpr maybe_t& operator=(utils::nothing_t /* unused */) noexcept
    {
        this->reset();
        return *this;
    }

    [[nodiscard]] constexpr bool has_value() const
    {
        return m_has_value;
    }

    explicit constexpr operator bool() const noexcept
    {
        return m_has_value;
    }

    constexpr value_type& operator*() & noexcept
    {
        return this->get();
    }

    constexpr const value_type& operator*() const& noexcept
    {
        return this->get();
    }

    constexpr value_type&& operator*() && noexcept
    {
        return std::move(this->get());
    }

    constexpr const value_type&& operator*() const&& noexcept
    {
        return std::move(this->get());
    }

    constexpr value_type* operator->() noexcept
    {
        return std::addressof(this->get());
    }

    constexpr const value_type* operator->() const noexcept
    {
        return std::addressof(this->get());
    }

    constexpr void reset()
    {
        if (has_value())
        {
            destroy();
        }
    }

    constexpr void swap(maybe_t& that) noexcept
    {
        using std::swap;

        if (this->has_value() && that.has_value())
        {
            swap(this->get(), that.get());
        }
        else if (this->has_value())
        {
            that.construct(std::move(this->get()));
            this->reset();
        }
        else if (that.has_value())
        {
            this->construct(std::move(that.get()));
            that.reset();
        }
    }

    constexpr friend void swap(maybe_t& lhs, maybe_t& rhs) noexcept
    {
        lhs.swap(rhs);
    }

    template <typename F, typename... Args>
    constexpr auto now(F&& fn, Args&&... args) &
    {
        using U = std::remove_cvref_t<std::invoke_result_t<F, Args..., value_type&>>;

        if (has_value())
        {
            return std::invoke(std::forward<F>(fn), std::forward<Args>(args)..., **this);
        }

        return U(utils::nothing);
    }

    template <typename F, typename... Args>
    constexpr auto now(F&& fn, Args&&... args) const&
    {
        using U = std::remove_cvref_t<std::invoke_result_t<F, Args..., value_type&>>;

        if (has_value())
        {
            return std::invoke(std::forward<F>(fn), std::forward<Args>(args)..., **this);
        }

        return U(utils::nothing);
    }

    template <typename F, typename... Args>
    constexpr auto now(F&& fn, Args&&... args) &&
    {
        using U = std::remove_cvref_t<std::invoke_result_t<F, Args..., value_type>>;

        if (has_value())
        {
            return std::invoke(std::forward<F>(fn), std::forward<Args>(args)..., std::move(**this));
        }

        return U(utils::nothing);
    }

    template <typename F, typename... Args>
    constexpr auto now(F&& fn, Args&&... args) const&&
    {
        using U = std::remove_cvref_t<std::invoke_result_t<F, Args..., value_type>>;

        if (has_value())
        {
            return std::invoke(std::forward<F>(fn), std::forward<Args>(args)..., std::move(**this));
        }

        return U(utils::nothing);
    }

    template <typename F, typename... Args>
        requires(std::is_same_v<std::invoke_result_t<F, Args..., value_type&>, void>)
    constexpr maybe_t& and_then(F&& fn, Args&&... args) &
    {
        if (has_value())
        {
            std::invoke(std::forward<F>(fn), std::forward<Args>(args)..., **this);
        }

        return *this;
    }

    template <typename F, typename... Args>
        requires(std::is_same_v<std::invoke_result_t<F, Args..., value_type&>, void>)
    constexpr const maybe_t& and_then(F&& fn, Args&&... args) const&
    {
        if (has_value())
        {
            std::invoke(std::forward<F>(fn), std::forward<Args>(args)..., **this);
        }

        return *this;
    }

    template <typename F, typename... Args>
        requires(std::is_same_v<std::invoke_result_t<F, Args..., value_type&>, void>)
    constexpr maybe_t&& and_then(F&& fn, Args&&... args) &&
    {
        if (has_value())
        {
            std::invoke(std::forward<F>(fn), std::forward<Args>(args)..., **this);
        }

        return std::move(*this);
    }

    template <typename F, typename... Args>
        requires(std::is_same_v<std::invoke_result_t<F, Args..., value_type&>, void>)
    constexpr maybe_t&& and_then(F&& fn, Args&&... args) const&&
    {
        if (has_value())
        {
            std::invoke(std::forward<F>(fn), std::forward<Args>(args)..., **this);
        }

        return std::move(*this);
    }

    template <typename F, typename... Args>
        requires(!std::is_same_v<std::invoke_result_t<F, Args..., value_type&>, void>)
    constexpr auto and_then(F&& fn, Args&&... args) &
    {
        using U = std::remove_cvref_t<std::invoke_result_t<F, Args..., value_type&>>;

        if (has_value())
        {
            return std::invoke(std::forward<F>(fn), std::forward<Args>(args)..., **this);
        }

        return U(utils::nothing);
    }

    template <typename F, typename... Args>
        requires(!std::is_same_v<std::invoke_result_t<F, Args..., value_type&>, void>)
    constexpr auto and_then(F&& fn, Args&&... args) const&
    {
        using U = std::remove_cvref_t<std::invoke_result_t<F, Args..., value_type&>>;

        if (has_value())
        {
            return std::invoke(std::forward<F>(fn), std::forward<Args>(args)..., **this);
        }

        return U(utils::nothing);
    }

    template <typename F, typename... Args>
        requires(!std::is_same_v<std::invoke_result_t<F, Args..., value_type&>, void>)
    constexpr auto and_then(F&& fn, Args&&... args) &&
    {
        using U = std::remove_cvref_t<std::invoke_result_t<F, Args..., value_type&&>>;

        if (has_value())
        {
            return std::invoke(std::forward<F>(fn), std::forward<Args>(args)..., std::move(**this));
        }

        return U(utils::nothing);
    }

    template <typename F, typename... Args>
        requires(!std::is_same_v<std::invoke_result_t<F, Args..., value_type&>, void>)
    constexpr auto and_then(F&& fn, Args&&... args) const&&
    {
        using U = std::remove_cvref_t<std::invoke_result_t<F, Args..., value_type&&>>;

        if (has_value())
        {
            return std::invoke(std::forward<F>(fn), std::forward<Args>(args)..., std::move(**this));
        }

        return U(utils::nothing);
    }

    template <typename F, typename... Args>
        requires(std::is_same_v<std::invoke_result_t<F, Args...>, void>)
    constexpr maybe_t& or_else(F&& fn, Args&&... args) &
    {
        if (!has_value())
        {
            std::invoke(std::forward<F>(fn), std::forward<Args>(args)...);
        }

        return *this;
    }

    template <typename F, typename... Args>
        requires(std::is_same_v<std::invoke_result_t<F, Args...>, void>)
    constexpr const maybe_t& or_else(F&& fn, Args&&... args) const&
    {
        if (!has_value())
        {
            std::invoke(std::forward<F>(fn), std::forward<Args>(args)...);
        }

        return *this;
    }

    template <typename F, typename... Args>
        requires(std::is_same_v<std::invoke_result_t<F, Args...>, void>)
    constexpr maybe_t&& or_else(F&& fn, Args&&... args) &&
    {
        if (!has_value())
        {
            std::invoke(std::forward<F>(fn), std::forward<Args>(args)...);
        }

        return std::move(*this);
    }

    template <typename F, typename... Args>
        requires(std::is_same_v<std::invoke_result_t<F, Args...>, void>)
    constexpr maybe_t&& or_else(F&& fn, Args&&... args) const&&
    {
        if (!has_value())
        {
            std::invoke(std::forward<F>(fn), std::forward<Args>(args)...);
        }

        return std::move(*this);
    }

    template <typename F, typename... Args>
        requires(!std::is_same_v<std::invoke_result_t<F, Args...>, void>)
    constexpr auto or_else(F&& fn, Args&&... args) &
    {
        if (!has_value())
        {
            return std::invoke(std::forward<F>(fn), std::forward<Args>(args)...);
        }

        return *this;
    }

    template <typename F, typename... Args>
        requires(!std::is_same_v<std::invoke_result_t<F, Args...>, void>)
    constexpr auto or_else(F&& fn, Args&&... args) const&
    {
        if (!has_value())
        {
            return std::invoke(std::forward<F>(fn), std::forward<Args>(args)...);
        }

        return *this;
    }

    template <typename F, typename... Args>
        requires(!std::is_same_v<std::invoke_result_t<F, Args...>, void>)
    constexpr auto or_else(F&& fn, Args&&... args) &&
    {
        if (!has_value())
        {
            return std::invoke(std::forward<F>(fn), std::forward<Args>(args)...);
        }

        return std::move(*this);
    }

    template <typename F, typename... Args>
        requires(!std::is_same_v<std::invoke_result_t<F, Args...>, void>)
    constexpr auto or_else(F&& fn, Args&&... args) const&&
    {
        if (!has_value())
        {
            return std::invoke(std::forward<F>(fn), std::forward<Args>(args)...);
        }

        return std::move(*this);
    }

private:
    template <typename... Args>
    constexpr void construct(Args&&... args)
    {
        ::new (std::addressof(this->get())) value_type(std::forward<Args>(args)...);
        m_has_value = true;
    }

    constexpr void destroy()
    {
        m_has_value = false;
        this->get().~value_type();
    }

    constexpr value_type& get() noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        return m_storage.value;
    }

    [[nodiscard]] constexpr const value_type& get() const noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        return m_storage.value;
    }

    union storage_t
    {
        struct empty_t
        {};

        constexpr storage_t()
            : nothing()
        {}

        template <typename... Args>
        explicit constexpr storage_t(utils::something_t /* unused */, Args&&... args)
            : value(std::forward<Args>(args)...)
        {}

        constexpr storage_t(const storage_t&) = delete;
        constexpr storage_t(storage_t&&) noexcept = delete;

        ~storage_t() {};

        constexpr storage_t& operator=(const storage_t&) = delete;
        constexpr storage_t& operator=(storage_t&&) noexcept = delete;

        value_type value;
        empty_t nothing;
    };

    storage_t m_storage;
    bool m_has_value = false;
};

template <typename T>
maybe_t(T) -> maybe_t<T>;

#endif  // MAYBE_HPP
