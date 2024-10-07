#ifndef MAYBE_HPP
#define MAYBE_HPP

/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

/// \cond
#include <utility>

/// \endcond

/*****************************************************************************/
/*** MACRO DEFINITIONS *******************************************************/

#if __cplusplus < 201703L
    #define MAYBE_NODISCARD
#else
    #define PAYLOAD_NODISCARD [[nodiscard]]
    #define MAYBE_NODISCARD   [[nodiscard]]
#endif  // __cplusplus

#if __cplusplus > 201703L
    #define PAYLOAD_CONSTEXPR_DESTRUCTOR constexpr
    #define MAYBE_CONSTEXPR_DESTRUCTOR   constexpr
#else
    #define PAYLOAD_CONSTEXPR_DESTRUCTOR
    #define MAYBE_CONSTEXPR_DESTRUCTOR
#endif  // __cplusplus

/*****************************************************************************/
/*** DATA TYPES **************************************************************/

namespace utils
{
    struct nothing_t
    {};

    inline constexpr nothing_t nothing{};

    template <typename T>
    class payload
    {
    public:
        using value_type = T;

        payload() = default;

        template <typename... Args>
        explicit constexpr payload(Args&&... args)
            : m_storage(std::forward<Args>(args)...)
            , m_has_value(true)
        {}

        constexpr payload(const payload& that)
            : m_has_value(that.m_has_value)
        {
            if (m_has_value)
            {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
                construct(that.m_storage.value);
            }
        }

        constexpr payload(payload&& that) noexcept
            : m_has_value(that.m_has_value)
        {
            if (m_has_value)
            {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
                construct(std::move(that.m_storage.value));

                that.m_has_value = false;
            }
        }

        PAYLOAD_CONSTEXPR_DESTRUCTOR
        ~payload()
        {
            if (m_has_value)
            {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
                m_storage.value.~value_type();
            }
        }

        constexpr payload& operator=(const payload& that)
        {
            if (this != std::addressof(that))
            {
                if (m_has_value && that.m_has_value)
                {
                    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
                    m_storage.value = that.m_storage.value;
                }
                else
                {
                    if (that.m_has_value)
                    {
                        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
                        construct(that.m_storage.value);
                    }
                    else
                    {
                        destroy();
                    }

                    m_has_value = that.m_has_value;
                }
            }

            return *this;
        }

        constexpr payload& operator=(payload&& that) noexcept
        {
            if (m_has_value && that.m_has_value)
            {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
                m_storage.value = std::move(that.m_storage.value);
            }
            else
            {
                if (that.m_has_value)
                {
                    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
                    construct(std::move(that.m_storage.value));
                }
                else
                {
                    destroy();
                }

                m_has_value = that.m_has_value;
                that.m_has_value = false;
            }

            return *this;
        }

        PAYLOAD_NODISCARD
        constexpr bool has_value() const
        {
            return m_has_value;
        }

        constexpr value_type* get()
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
            return std::addressof(m_storage.value);
        }

    private:
        template <typename... Args>
        void construct(Args&&... args)
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
            ::new (std::addressof(m_storage.value)) value_type(std::forward<Args>(args)...);
        }

        void destroy()
        {
            if (m_has_value)
            {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
                m_storage.value.~value_type();
            }
        }

        union storage
        {
            struct empty_t
            {};

            constexpr storage()
                : nothing()
            {}

            template <typename... Args>
            explicit constexpr storage(Args&&... args)
                : value(std::forward<Args>(args)...)
            {}

            constexpr storage(const storage&) = delete;
            constexpr storage(storage&&) noexcept = delete;

            ~storage() {};

            constexpr storage& operator=(const storage&) = delete;
            constexpr storage& operator=(storage&&) noexcept = delete;

            value_type value;
            empty_t nothing;
        };

        storage m_storage;
        bool m_has_value = false;
    };

    template <typename T>
    class maybe
    {
    public:
        using value_type = T;

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        constexpr maybe(const value_type& value)
            : m_payload(value)
        {}

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        constexpr maybe(value_type&& value)
            : m_payload(std::move(value))
        {}

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        constexpr maybe(utils::nothing_t /* unused */)
        {}

        constexpr maybe(const maybe& that) = default;
        constexpr maybe(maybe&& that) noexcept = default;

        MAYBE_CONSTEXPR_DESTRUCTOR
        ~maybe() = default;

        constexpr maybe& operator=(const maybe& that) = default;
        constexpr maybe& operator=(maybe&& that) noexcept = default;

        MAYBE_NODISCARD
        constexpr bool has_value() const
        {
            return m_payload.has_value();
        }

        template <typename F>
        constexpr auto and_then(F&& func)
        {
            if (has_value())
            {
                return std::forward<F>(func)(**this);
            }

            return *this;
        }

        template <typename F>
        constexpr auto or_else(F&& func)
        {
            if (!has_value())
            {
                return std::forward<F>(func)();
            }

            return *this;
        }

        explicit constexpr operator bool() const noexcept
        {
            return m_payload.has_value();
        }

        constexpr value_type& operator*()
        {
            return *(m_payload.get());
        }

        constexpr value_type* operator->()
        {
            return m_payload.get();
        }

    private:
        payload<value_type> m_payload;
    };
}  // namespace utils

#undef PAYLOAD_NODISCARD
#undef PAYLOAD_NODISCARD

#undef PAYLOAD_CONSTEXPR_DESTRUCTOR
#undef MAYBE_CONSTEXPR_DESTRUCTOR

#endif  // MAYBE_HPP
