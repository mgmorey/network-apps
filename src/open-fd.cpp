#include "network/open.h"       // Fd, Result, SockAddr, fd_null,
                                // open(), operator<<()
#include "network/error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
#include "network/sockaddr.h"   // get_length(), get_pointer(),
                                // is_valid(), operator<<()

#include <cassert>      // assert()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::Result
Network::open(const OpenHandler& handler, Fd fd,
              const SockAddr& addr,
              bool verbose)
{
    Result result;
    assert(is_valid(addr, verbose));
    const auto addr_len {get_length(addr)};
    const auto addr_ptr {get_pointer(addr)};

    if (verbose) {
        std::cerr << "Calling "
                  << handler.second
                  << '('
                  << fd
                  << ", "
                  << addr
                  << ", "
                  << static_cast<int>(addr_len)
                  << ')'
                  << std::endl;
    }

    reset_last_error();

    if (handler.first(fd, addr_ptr, addr_len) == socket_error) {
        const auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to "
            << handler.second
            << '('
            << fd
            << ", "
            << addr
            << ") failed with error "
            << error
            << ": "
            << format_error(error);
        result = {error, oss.str()};
    }

    return result;
}
