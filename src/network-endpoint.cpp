#include "network-endpoint.h"   // Address, Endpoint, EndpointResult,
                                // Result, to_string()
#include "network-sockaddr.h"   // SockAddr, operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#else
#include <netdb.h>      // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::EndpointResult::EndpointResult(const Address& address,
                                        int flags,
                                        bool verbose) :
    code(0),
    host(NI_MAXHOST),
    serv(NI_MAXSERV)
{
    SockAddr sockaddr(address);
    code = ::getnameinfo(sockaddr,
                         sockaddr.length(),
                         &host[0], host.size(),
                         &serv[0], serv.size(),
                         flags);

    if (verbose) {
        std::cerr << "Invoking getnameinfo("
                  << to_string(address)
                  << ", ...)"
                  << std::endl;
    }

    if (code != 0) {
        std::ostringstream os;
        os << "getnameinfo(...) returned "
           << code
           << " ("
           << ::gai_strerror(code)
           << ')';
        error = os.str();
    }
}

Network::Endpoint Network::EndpointResult::endpoint() const
{
    return Endpoint(hostname(), service());
}

Network::Hostname Network::EndpointResult::hostname() const
{
    return host;
}

Network::Service Network::EndpointResult::service() const
{
    return serv;
}

Network::Result Network::EndpointResult::result() const
{
    return Result(code, error);
}

Network::EndpointResult Network::to_endpoint(const Address& address,
                                       bool numeric,
                                       bool verbose)
{
    int flags = numeric ? NI_NUMERICHOST | NI_NUMERICSERV : 0;
    return EndpointResult(address, flags, verbose);
}

Network::Hostname Network::to_hostname(const Address& address,
                                       bool verbose)
{
    return to_endpoint(address, false, verbose).hostname();
}

Network::Hostname Network::to_string(const Address& address,
                                     bool verbose)
{
    return to_endpoint(address, true, verbose).hostname();
}
