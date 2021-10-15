#include "network-endpoint.h"   // Endpoint, EndpointResult, Result,
                                // SockAddr, get_endpoint()
#include "network-buffer.h"     // Buffer
#include "network-sockaddr.h"   // get_length(), get_pointer(),
                                // is_valid(), operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#else
#include <netdb.h>      // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#endif

#include <algorithm>    // std::max()
#include <cassert>      // assert()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::EndpointResult
Network::get_endpoint(const SockAddr& sock_addr, int flags, bool verbose)
{
    Result result;
    Buffer host {NI_MAXHOST};
    Buffer serv {NI_MAXSERV};
    assert(is_valid(sock_addr, verbose));
    const auto addr_ptr {get_pointer(sock_addr)};
    const auto addr_len {get_length(sock_addr)};

    if (verbose) {
        std::cerr << "Calling getnameinfo("
                  << sock_addr
                  << ", "
                  << addr_len
                  << ", ..., "
                  << flags
                  << ')'
                  << std::endl;
    }

    const auto error {::getnameinfo(addr_ptr, addr_len,
                                    &host[0], host.size(),
                                    &serv[0], serv.size(),
                                    flags)};

    if (error) {
        std::ostringstream oss;
        oss << "Call to getnameinfo("
            << sock_addr
            << ", "
            << addr_len
            << ", ..., "
            << flags
            << ") returned "
            << error
            << " ("
            << ::gai_strerror(error)
            << ')';
        result = {error, oss.str()};
    }

    const Endpoint endpoint(host, serv);
    return EndpointResult(endpoint, result);
}

Network::EndpointResult
Network::get_endpoint(const SockAddr& sock_addr, bool numeric, bool verbose)
{
    const int flags {numeric ? NI_NUMERICHOST | NI_NUMERICSERV : 0};
    return get_endpoint(sock_addr, flags, verbose);
}
