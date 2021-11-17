#include "network/name.h"       // Fd, GetNameHandler, Result,
                                // SockAddrResult, get_name()
#include "network/error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
#include "network/sockaddr.h"   // get_length(), get_pointer(),
                                // get_sockaddr(), is_valid()

#include <algorithm>    // std::max()
#include <cassert>      // assert()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::SockAddrResult Network::get_name(const GetNameHandler& handler, Fd fd,
                                          bool verbose)
{
    Result result;
    auto addr {get_sockaddr()};
    auto addr_len {get_length(addr)};
    auto *addr_ptr {get_pointer(addr)};

    if (verbose) {
        std::cerr << "Calling "
                  << handler.second
                  << '('
                  << fd
                  << ", "
                  << addr
                  << ", "
                  << static_cast<int>(addr_len)
                  << ", ...)"
                  << std::endl;
    }

    reset_last_error();

    if (handler.first(fd, addr_ptr, &addr_len)) {
        auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to "
            << handler.second
            << '('
            << fd
            << ", "
            << addr
            << ", "
            << static_cast<int>(addr_len)
            << ", ...) failed with error "
            << error
            << ": "
            << format_error(error);
        return Result(error, oss.str());
    }

    addr.resize(addr_len);
    assert(is_valid(addr, verbose));
    return addr;
}
