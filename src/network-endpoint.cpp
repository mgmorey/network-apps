#include "network-address.h"    // Address, Endpoint, EndpointResult,
                                // Result, operator<<(), to_endpoint()
#include "network-buffer.h"     // Buffer
#include "network-sockaddr.h"   // SockAddr, get_sockaddr_length(),
                                // get_sockaddr_pointer()

#ifdef _WIN32
#include <ws2tcpip.h>   // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#else
#include <netdb.h>      // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#endif

#include <cassert>      // assert()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::EndpointResult
Network::Address::to_endpoint(int t_flags, bool t_verbose) const
{
    assert(!empty());
    std::string message;
    SockAddr addr {*this};
    Buffer host {NI_MAXHOST};
    Buffer serv {NI_MAXSERV};
    auto addr_len {get_length(addr)};
    auto addr_ptr {get_pointer(addr)};
    const auto error {::getnameinfo(addr_ptr, addr_len,
                                    &host[0], host.size(),
                                    &serv[0], serv.size(),
                                    t_flags)};

    if (t_verbose) {
        std::cerr << "Calling getnameinfo("
                  << *this
                  << ", ...)"
                  << std::endl;
    }

    if (error != 0) {
        std::ostringstream oss;
        oss << "Call to getnameinfo("
            << *this
            << ", ...) returned "
            << error
            << " ("
            << ::gai_strerror(error)
            << ')';
        message = oss.str();
    }

    assert(error == 0 ?
           message == "" :
           message != "");
    return EndpointResult(Endpoint(host, serv), {error, message});
}

Network::EndpointResult
Network::Address::to_endpoint(bool t_numeric, bool t_verbose) const
{
    const int flags {t_numeric ? NI_NUMERICHOST | NI_NUMERICSERV : 0};
    return to_endpoint(flags, t_verbose);
}
