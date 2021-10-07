#include "network-peername.h"   // AddressBuffer, AddressResult, Fd,
                                // Result, get_peername()
#include "network-error.h"      // format_error(), get_last_error(),
                                // ureset_last_error()
#include "network-sockaddr.h"   // get_pointer(), get_sockaddr()
#include "network-socklen.h"    // socklen_type
#include "network-types.h"      // SockAddr

#ifdef _WIN32
#include <winsock2.h>   // getpeername()
#else
#include <sys/socket.h> // getpeername()
#endif

#include <algorithm>    // std::max()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::AddressResult Network::get_peername(Fd fd, bool verbose)
{
    if (verbose) {
        std::cerr << "Calling getpeername("
                  << fd
                  << ", ...)"
                  << std::endl;
    }

    Result result;
    auto addr {get_sockaddr()};
    auto addr_ptr {get_pointer(addr)};
    auto addr_len {get_length(addr)};
    // cppcheck-suppress variableScope
    auto error {reset_last_error()};

    if (::getpeername(fd, addr_ptr, &addr_len)) {
        error = get_last_error();
        std::ostringstream oss;
        oss << "Call to getpeername("
            << fd
            << ", ...) failed with error "
            << error
            << ": "
            << format_error(error);
        result = {error, oss.str()};
    }
    else {
        addr = addr.substr(0, addr_len);
        assert(is_valid(addr));
    }

    const Address address(addr);
    return AddressResult(address, result);
}
