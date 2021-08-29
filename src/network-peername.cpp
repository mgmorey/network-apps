#include "network-peername.h"       // AddressBuffer, AddressResult,
                                    // Result, get_peername(),
                                    // sock_fd_type

#ifdef _WIN32
#include <winsock2.h>   // getpeername(), struct sockaddr_storage
#else
#include <sys/socket.h> // getpeername(), struct sockaddr_storage,
                        // struct sockaddr_un
#endif

#include <algorithm>    // std::max()
#include <cerrno>       // errno
#include <cstddef>      // std::size_t
#include <cstring>      // std::strerror()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::AddressBuffer::AddressBuffer() :
    Buffer(capacity())
{
}

sockaddr* Network::AddressBuffer::addr()
{
    return reinterpret_cast<sockaddr*>(&(*this)[0]);
}

socklen_t Network::AddressBuffer::addrlen() const
{
    return static_cast<socklen_t>(size());
}

std::size_t Network::AddressBuffer::capacity()
{
    auto size = sizeof(sockaddr_storage);
#ifndef _WIN32
    size = std::max(size, sizeof(sockaddr_un));
#endif
    return size;
}

Network::AddressResult Network::get_peername(sock_fd_type sock_fd,
                                             bool verbose)
{
    if (verbose) {
        std::cerr << "Invoking getpeername("
                  << sock_fd
                  << ", ...)"
                  << std::endl;
    }

    std::string error;
    AddressBuffer buffer;
    sockaddr* addr = buffer.addr();
    socklen_t addrlen = buffer.addrlen();
    const int code = ::getpeername(sock_fd, addr, &addrlen);

    if (code != 0) {
        std::ostringstream oss;
        oss << "getpeername("
            << sock_fd
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
