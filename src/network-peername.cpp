#include "network-peername.h"       // AddressResult, get_peername()

#ifdef _WIN32
#include <winsock2.h>   // getpeername(), struct sockaddr, struct
                        // sockaddr_storage
#else
#include <sys/socket.h> // getpeername(), struct sockaddr, struct
                        // sockaddr_storage, struct sockaddr_un
#endif

#include <cerrno>       // errno
#include <cstddef>      // std::size_t
#include <cstring>      // std::strerror()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::AddressResult Network::get_peername(sock_fd_type fd, bool verbose)
{
    if (verbose) {
        std::cerr << "Invoking getpeername("
                  << fd
                  << ", ...)"
                  << std::endl;
    }

    std::size_t size = sizeof(sockaddr_storage);

#ifndef _WIN32
    if (size < sizeof(sockaddr_un)) {
        size = sizeof(sockaddr_un);
    }
#endif

    std::string error;
    std::string buffer(size, '\0');
    sockaddr* addr = reinterpret_cast<sockaddr*>(&buffer[0]);
    socklen_t addrlen = buffer.size();
    const int code = ::getpeername(fd, addr, &addrlen);

    if (code != 0) {
        std::ostringstream oss;
        oss << "getpeername("
            << fd
            << ", ...) returned "
            << code
            << ": "
            << std::strerror(errno)
            << " (errno = "
            << errno
            << ')';
        error = oss.str();
    }

    return AddressResult(Address(addr, addrlen), Result(code, error));
}
