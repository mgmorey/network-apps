#include "network-peername.h"       // AddressBuffer, AddressResult,
                                    // Result, SocketFd,
                                    // get_peername()

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
#include <string>       // std::string

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
    auto size {sizeof(sockaddr_storage)};
#ifndef _WIN32
    size = std::max(size, sizeof(sockaddr_un));
#endif
    return size;
}

Network::AddressResult Network::get_peername(SocketFd socket_fd,
                                             bool verbose)
{
    if (verbose) {
        std::cerr << "Invoking getpeername("
                  << socket_fd
                  << ", ...)"
                  << std::endl;
    }

    errno = 0;
    std::string error;
    AddressBuffer buffer;
    auto addr {buffer.addr()};
    auto addrlen {buffer.addrlen()};
    const auto fd {static_cast<sock_fd_type>(socket_fd)};
    const auto code {::getpeername(fd, addr, &addrlen)};

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
