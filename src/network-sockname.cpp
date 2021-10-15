#include "network-sockname.h"   // Fd, Result, SockAddr,
                                // get_sockname()
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
#include "network-sockaddr.h"   // get_length(), get_pointer(),
                                // get_sockaddr(), is_valid()

#ifdef _WIN32
#include <winsock2.h>   // getsockname()
#else
#include <sys/socket.h> // getsockname()
#endif

#include <algorithm>    // std::max()
#include <cassert>      // assert()
#include <cstddef>      // std::size_t
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

std::pair<Network::SockAddr, Network::Result>
Network::get_sockname(Fd fd, bool verbose)
{
    Result result;
    auto sock_addr {get_sockaddr()};
    auto addr_ptr {get_pointer(sock_addr)};
    auto addr_len {get_length(sock_addr)};
    // cppcheck-suppress variableScope
    auto error {reset_last_error()};

    if (verbose) {
        std::cerr << "Calling getsockname("
                  << fd
                  << ", "
                  << sock_addr
                  << ", "
                  << static_cast<int>(addr_len)
                  << ", ...)"
                  << std::endl;
    }

    if (::getsockname(fd, addr_ptr, &addr_len)) {
        error = get_last_error();
        std::ostringstream oss;
        oss << "Call to getsockname("
            << fd
            << ", "
            << sock_addr
            << ", "
            << static_cast<int>(addr_len)
            << ", ...) failed with error "
            << error
            << ": "
            << format_error(error);
        result = {error, oss.str()};
    }
    else {
        sock_addr.resize(addr_len);
        assert(is_valid(sock_addr, verbose));
    }

    return std::pair(sock_addr, result);
}
