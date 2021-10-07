#include "network-peername.h"   // AddressBuffer, AddressResult, Fd,
                                // Result, get_peername()
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
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

    std::string message;
    auto addr {get_sockaddr()};
    auto addr_ptr {get_pointer(addr)};
    auto addr_len {static_cast<socklen_t>(addr.size())};
    auto error {reset_last_error()};
    const auto code {::getpeername(fd, addr_ptr, &addr_len)};

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
    else {
        addr = addr.substr(0, addr_len);
        assert(is_valid(addr));
    }

    assert(error == 0 ?
           message == "" :
           message != "");
    const Address address(addr);
    return AddressResult(address, {error, message});
}
