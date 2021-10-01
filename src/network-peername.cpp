#include "network-peername.h"   // AddressBuffer, AddressResult,
                                // Result, SocketFd,
                                // get_peername()
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()

#ifdef _WIN32
#include <winsock2.h>   // getpeername(), struct sockaddr_storage
#else
#include <sys/socket.h> // getpeername(), struct sockaddr_storage,
                        // struct sockaddr_un
#endif

#include <algorithm>    // std::max()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
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
        std::cerr << "Calling getpeername("
                  << socket_fd
                  << ", ...)"
                  << std::endl;
    }

    std::string error;
    AddressBuffer buffer;
    auto addr {buffer.addr()};
    auto addrlen {buffer.addrlen()};
    auto code {reset_last_error()};
    const auto fd {static_cast<fd_type>(socket_fd)};

    if (::getpeername(fd, addr, &addrlen) != 0) {
        code = get_last_error();
        std::ostringstream oss;
        oss << "Call to getpeername("
            << fd
            << ", ...) failed with error "
            << code
            << ": "
            << format_error(code);
        error = oss.str();
    }

    Result result {code, error};
    assert(result.result() ?
           result.string() != "" :
           result.string() == "");
    return AddressResult(Address(addr, addrlen), result);
}
