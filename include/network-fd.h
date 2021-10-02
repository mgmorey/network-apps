#ifndef NETWORK_FD_H
#define NETWORK_FD_H

#ifdef _WIN32
#include <winsock2.h>   // INVALID_SOCKET, SOCKET
#endif

#include <ostream>     // std::ostream

#ifndef INVALID_SOCKET
#ifdef _WIN32
#define INVALID_SOCKET	(~0)
#else
#define INVALID_SOCKET	(-1)
#endif
#endif

namespace Network
{
#ifdef _WIN32
    typedef SOCKET fd_type;
#else
    typedef int fd_type;
#endif

    enum { fd_null = INVALID_SOCKET };

    class Fd
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Fd& fd);

    public:
        explicit Fd(fd_type t_fd)
        {
            m_value = t_fd;
        }

        explicit operator fd_type() const
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

    inline std::ostream& operator<<(std::ostream& os,
                                    const Fd& fd)
    {
        os << fd.m_value;
        return os;
    }
}

#endif
