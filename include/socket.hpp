#ifndef SOCKET_HPP
#define SOCKET_HPP

/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <unistd.h>

/// \cond
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>
#include <tuple>

/// \endcond

/*****************************************************************************/
/*** CLASSES *****************************************************************/

class socket_t
{
    static constexpr int default_backlog_length = 128;

public:
    socket_t()
        : m_descriptor(::socket(AF_INET, SOCK_STREAM, 0))
    {}

    socket_t(socket_t&& that) noexcept
        : m_descriptor(that.m_descriptor)
    {
        that.m_descriptor = -1;
    }

    socket_t& operator=(socket_t&& that) noexcept
    {
        if (this != std::addressof(that))
        {
            this->m_descriptor = that.m_descriptor;
            that.m_descriptor = -1;
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

        int enable = 1;

        ::setsockopt(m_descriptor, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
        ::setsockopt(m_descriptor, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(enable));

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
        if (m_descriptor != -1)
        {
            ::shutdown(m_descriptor, AF_INET);
            ::close(m_descriptor);

            m_descriptor = -1;
        }
    }

    [[nodiscard]] std::optional<std::size_t> send(std::string_view message) const
    {
        return send(message.data(), message.length());
    }

    [[nodiscard]] std::optional<std::size_t> send(const void* data, std::size_t length) const
    {
        auto result = ::send(m_descriptor, data, length, 0);

        if (result == -1)
        {
            return std::nullopt;
        }

        return static_cast<std::size_t>(result);
    }

    [[nodiscard]] std::optional<std::size_t> recv(void* data, std::size_t length) const
    {
        auto result = ::recv(m_descriptor, data, length, 0);

        if (result == 0 || result == -1)
        {
            return std::nullopt;
        }

        return static_cast<std::size_t>(result);
    }

private:
    explicit socket_t(int descriptor)
        : m_descriptor(descriptor)
    {}

    int m_descriptor;
};

#endif  // SOCKET_HPP
