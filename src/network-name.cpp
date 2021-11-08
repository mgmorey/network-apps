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

Network::SockAddrResult
Network::get_name(const Fd& fd,
                  get_name_function_type get_name_function_ptr,
                  const std::string& get_name_function_str,
                  bool verbose)
{
    Result result;
    auto sock_addr {get_sockaddr()};
    auto addr_ptr {get_pointer(sock_addr)};
    auto addr_len {get_length(sock_addr)};

    if (verbose) {
        std::cerr << "Calling "
                  << get_name_function_str
                  << '('
                  << fd
                  << ", "
                  << sock_addr
                  << ", "
                  << static_cast<int>(addr_len)
                  << ", ...)"
                  << std::endl;
    }

    reset_last_error();

    if (get_name_function_ptr(fd, addr_ptr, &addr_len)) {
        auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to "
            << get_name_function_str
            << '('
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
