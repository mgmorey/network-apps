#include "network-endpoint.h"   // Endpoint, EndpointResult, Result,
                                // get_endpoint()
#include "network-address.h"    // Address, operator<<()
#include "network-buffer.h"     // Buffer
#include "network-sockaddr.h"   // get_length(), get_pointer()
#include "network-types.h"      // SockAddr

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
Network::get_endpoint(const Address& address, int flags, bool verbose)
{
    assert(!address.empty());
    std::string message;
    SockAddr addr {address};
    Buffer host {NI_MAXHOST};
    Buffer serv {NI_MAXSERV};
    const auto addr_ptr {get_pointer(addr)};
    const auto addr_len {get_length(addr)};
    const auto error {::getnameinfo(addr_ptr, addr_len,
                                    &host[0], host.size(),
                                    &serv[0], serv.size(),
                                    flags)};

    if (verbose) {
        std::cerr << "Calling getnameinfo("
                  << address
                  << ", ...)"
                  << std::endl;
    }

    if (error != 0) {
        std::ostringstream oss;
        oss << "Call to getnameinfo("
            << address
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
    const Endpoint endpoint(Endpoint(host, serv));
    return EndpointResult(endpoint, {error, message});
}

Network::EndpointResult
Network::get_endpoint(const Address& address, bool numeric, bool verbose)
{
    const int flags {numeric ? NI_NUMERICHOST | NI_NUMERICSERV : 0};
    return get_endpoint(address, flags, verbose);
}
