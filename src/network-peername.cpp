#include "network-peername.h"   // AddressBuffer, AddressResult, Fd,
                                // Result, get_peername()
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

Network::Address::sock_len_type Network::AddressBuffer::addrlen() const
{
    return static_cast<Network::Address::sock_len_type>(size());
}

std::size_t Network::AddressBuffer::capacity()
{
    auto size {sizeof(sockaddr_storage)};
#ifndef _WIN32
    size = std::max(size, sizeof(sockaddr_un));
#endif
    return size;
}

Network::AddressResult Network::get_peername(Fd fd, bool verbose)
{
    if (verbose) {
        std::cerr << "Calling getpeername("
                  << fd
                  << ", ...)"
                  << std::endl;
    }

    std::string message;
    AddressBuffer buffer;
    auto address {buffer.addr()};
    auto addrlen {buffer.addrlen()};
    auto error {reset_last_error()};
    const auto code {::getpeername(fd, address, &addrlen)};

    if (code != 0) {
        error = get_last_error();
        std::ostringstream oss;
        oss << "Call to getpeername("
            << fd
            << ", ...) failed with error "
            << error
            << ": "
            << format_error(error);
        message = oss.str();
    }

    assert(error == 0 ?
           message == "" :
           message != "");
    return {{address, addrlen}, {error, message}};
}
