#ifndef NETWORK_FD_H
#define NETWORK_FD_H

#ifdef _WIN32
#include <winsock2.h>   // INVALID_SOCKET, SOCKET
#endif

#include <ostream>     // std::ostream

#ifndef INVALID_SOCKET
#define INVALID_SOCKET	(-1)
#endif

namespace Network
{
#ifdef _WIN32
    typedef SOCKET sock_fd_type;
#else
    typedef int sock_fd_type;
#endif

    enum { sock_fd_null = INVALID_SOCKET };

    class SocketFd
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const SocketFd& socket_fd);

    public:
        explicit SocketFd(sock_fd_type t_sock_fd)
        {
            m_value = t_sock_fd;
        }

        explicit operator sock_fd_type() const
        {
            return m_value;
        }

        explicit operator bool() const
        {
            return m_value != sock_fd_null;;
        }

    private:
        sock_fd_type m_value {sock_fd_null};
    };

    inline std::ostream& operator<<(std::ostream& os,
                                    const SocketFd& socket_fd)
    {
        os << socket_fd.m_value;
        return os;
    }
}

#endif
