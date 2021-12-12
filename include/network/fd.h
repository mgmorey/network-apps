#ifndef NETWORK_FD_H
#define NETWORK_FD_H

#ifdef _WIN32
#include <winsock2.h>       // INVALID_SOCKET, SOCKET
#endif

#include <ostream>     // std::ostream

namespace Network
{
#ifdef _WIN32
    using fd_type = SOCKET;
#else
    using fd_type = int;
#endif

#ifndef INVALID_SOCKET
#ifdef _WIN32
    constexpr fd_type INVALID_SOCKET {~0};
#else
    constexpr fd_type INVALID_SOCKET {-1};
#endif
#endif

    enum { fd_null = INVALID_SOCKET };

    class Fd
    {
        friend auto operator<<(std::ostream& os, Fd fd) -> std::ostream&;

    public:
        Fd() = default;

        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE
        Fd(fd_type t_fd) :
            m_value(t_fd)
        {
        }

        operator fd_type() const  // NOLINT
        {
            return m_value;
        }

        explicit operator bool() const
        {
            return m_value != fd_null;;
        }

    private:
        fd_type m_value {fd_null};
    };

    inline auto operator<<(std::ostream& os, Fd fd) -> std::ostream&
    {
        return os << fd.m_value;
    }
}

#endif
