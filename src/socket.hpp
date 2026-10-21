#ifndef SOCKET_HPP
#define SOCKET_HPP

/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>

    #include <poll.h>
    #include <unistd.h>
#endif

/// \cond
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>
#include <tuple>

/// \endcond

#if defined(_WIN32)
// NOLINTNEXTLINE(cert-err58-cpp)
static const bool initialized = [] {
    std::ignore = std::atexit([] {
        if (initialized)
        {
            WSACleanup();
        }
    });

    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);

    return true;
}();
#endif

/*****************************************************************************/
/*** CLASSES *****************************************************************/

class socket_t
{
    static constexpr int default_backlog_length = 128;

#if defined(_WIN32)
    using descriptor_t = SOCKET;
    static constexpr descriptor_t invalid_descriptor = INVALID_SOCKET;
#else
    using descriptor_t = int;
    static constexpr descriptor_t invalid_descriptor = -1;
#endif

public:
    socket_t()
        : m_descriptor(::socket(AF_INET, SOCK_STREAM, 0))
    {}

    socket_t(socket_t&& that) noexcept
        : m_descriptor(that.m_descriptor)
    {
        that.m_descriptor = invalid_descriptor;
    }

    socket_t& operator=(socket_t&& that) noexcept
    {
        if (this != std::addressof(that))
        {
            this->m_descriptor = that.m_descriptor;
            that.m_descriptor = invalid_descriptor;
        }

        return *this;
    }

    ~socket_t()
    {
        close();
    }

    socket_t(const socket_t& /* that */) = delete;
    socket_t& operator=(const socket_t& /* that */) = delete;

    void bind(std::string_view addr, uint16_t port) const
    {
        sockaddr_in socket{};

        socket.sin_family = AF_INET;
        socket.sin_port = htons(port);

        // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage)
        ::inet_pton(AF_INET, addr.data(), &socket.sin_addr);

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        auto* sock_addr = reinterpret_cast<sockaddr*>(&socket);

#if defined(__WIN32)
        char enable = 1;
#else
        int enable = 1;
#endif

        ::setsockopt(m_descriptor, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

#if defined(__linux__) || defined(__APPLE__)
        ::setsockopt(m_descriptor, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(enable));
#endif

        std::ignore = ::bind(m_descriptor, sock_addr, sizeof(socket));
    }

    void listen(int backlog = default_backlog_length) const
    {
        std::ignore = ::listen(m_descriptor, backlog);
    }

    [[nodiscard]] socket_t accept() const
    {
        sockaddr_in socket{};
        socklen_t socket_length = sizeof(socket);

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        auto* socket_addr = reinterpret_cast<sockaddr*>(&socket);

        return socket_t(::accept(m_descriptor, socket_addr, &socket_length));
    }

    void connect(std::string_view addr, uint16_t port) const
    {
        sockaddr_in socket{};

        socket.sin_family = AF_INET;
        socket.sin_port = htons(port);

        // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage)
        ::inet_pton(AF_INET, addr.data(), &socket.sin_addr);

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        auto* socket_addr = reinterpret_cast<sockaddr*>(&socket);

        std::ignore = ::connect(m_descriptor, socket_addr, sizeof(socket));
    }

    void close()
    {
        if (m_descriptor != invalid_descriptor)
        {
#if defined(__WIN32)
            ::shutdown(m_descriptor, SD_BOTH);
            ::closesocket(m_descriptor);
#else
            ::shutdown(m_descriptor, AF_INET);
            ::close(m_descriptor);
#endif

            m_descriptor = invalid_descriptor;
        }
    }

    [[nodiscard]] std::optional<std::size_t> send(std::string_view message) const
    {
        return send(message.data(), message.length());
    }

    [[nodiscard]] std::optional<std::size_t> send(const void* data, std::size_t length) const
    {
#if defined(__WIN32)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        const auto* buffer = reinterpret_cast<const char*>(data);
        auto size = static_cast<int>(length);
#else
        const auto* buffer = data;
        auto size = static_cast<ssize_t>(length);
#endif

        auto result = ::send(m_descriptor, buffer, size, 0);

        if (result == -1)
        {
            return std::nullopt;
        }

        return static_cast<std::size_t>(result);
    }

    [[nodiscard]] std::optional<std::size_t> recv(void* data, std::size_t length) const
    {
#if defined(__WIN32)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        auto* buffer = reinterpret_cast<char*>(data);
        auto size = static_cast<int>(length);
#else
        auto* buffer = data;
        auto size = static_cast<ssize_t>(length);
#endif

        auto result = ::recv(m_descriptor, buffer, size, 0);

        if (result == 0 || result == -1)
        {
            return std::nullopt;
        }

        return static_cast<std::size_t>(result);
    }

    [[nodiscard]] std::optional<std::size_t> pool(int timeout) const
    {
        pollfd pfd{};

        pfd.fd = m_descriptor;
        pfd.events = POLLIN;
        pfd.revents = 0;

#if defined(__WIN32)
        auto result = WSAPoll(std::addressof(pfd), 1, timeout);
#else
        auto result = ::poll(std::addressof(pfd), 1, timeout);
#endif
        if (result == -1)
        {
            return std::nullopt;
        }

        return static_cast<std::size_t>(result);
    }

private:
    explicit socket_t(descriptor_t descriptor)
        : m_descriptor(descriptor)
    {}

    descriptor_t m_descriptor;
};

#endif  // SOCKET_HPP
