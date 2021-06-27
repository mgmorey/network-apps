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

Network::EndpointResult::EndpointResult(const Host& host,
                                        int flags,
                                        bool verbose) :
    code(0),
    host_buffer(NI_MAXHOST),
    service_buffer(NI_MAXSERV)
{
    SockAddr sockaddr(host);
    code = ::getnameinfo(sockaddr,
                         sockaddr.length(),
                         &host_buffer[0], host_buffer.size(),
                         &service_buffer[0], service_buffer.size(),
                         flags);

    if (verbose) {
        std::cerr << "Invoking getnameinfo("
                  << to_string(host)
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
    return host_buffer;
}

Network::Service Network::EndpointResult::service() const
{
    return service_buffer;
}

Network::Result Network::EndpointResult::result() const
{
    return Result(code, error);
}

Network::EndpointResult Network::to_endpoint(const Host& host,
                                             bool numeric,
                                             bool verbose)
{
    int flags = numeric ? NI_NUMERICHOST | NI_NUMERICSERV : 0;
    return EndpointResult(host, flags, verbose);
}

Network::Hostname Network::to_hostname(const Host& host,
                                       bool verbose)
{
    return to_endpoint(host, false, verbose).hostname();
}

Network::Hostname Network::to_string(const Host& host,
                                     bool verbose)
{
    return to_endpoint(host, true, verbose).hostname();
}
