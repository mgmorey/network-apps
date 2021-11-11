#include "network-name.h"       // Fd, Result, SockAddr
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
#include "network-sockaddr.h"   // get_length(), get_pointer(),
                                // get_sockaddr(), is_valid()

#ifdef _WIN32
#include <winsock2.h>   // getname()
#else
#include <sys/socket.h> // getname()
#endif

#include <algorithm>    // std::max()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::SockAddrResult Network::get_name(const GetNameMethod& method, Fd fd,
                                          bool verbose)
{
    Result result;
    auto addr {get_sockaddr()};
    auto addr_len {get_length(addr)};
    auto addr_ptr {get_pointer(addr)};

    if (verbose) {
        std::cerr << "Calling "
                  << method.second
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

    if (method.first(fd, addr_ptr, &addr_len)) {
        auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to "
            << method.second
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
    else {
        addr.resize(addr_len);
        assert(is_valid(addr, verbose));
        return addr;
    }
}
