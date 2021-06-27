#include "network-endpoint.h"   // Endpoint, EndpointResult, Result,
                                // SockAddr, to_string()

#ifdef _WIN32
#include <ws2tcpip.h>   // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#else
#include <netdb.h>      // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::EndpointResult::EndpointResult(const SockAddr& address,
                                        int flags,
                                        bool verbose) :
    code(0),
    host_buffer(NI_MAXHOST),
    service_buffer(NI_MAXSERV)
{
    code = ::getnameinfo(address, address.length(),
                         &host_buffer[0], host_buffer.size(),
                         &service_buffer[0], service_buffer.size(),
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

Network::EndpointResult Network::to_endpoint(const SockAddr& address,
                                             bool numeric,
                                             bool verbose)
{
    int flags = numeric ? NI_NUMERICHOST | NI_NUMERICSERV : 0;
    return EndpointResult(address, flags, verbose);
}

Network::Hostname Network::to_hostname(const SockAddr& address,
                                       bool verbose)
{
    return to_endpoint(address, false, verbose).hostname();
}

Network::Hostname Network::to_string(const SockAddr& address,
                                     bool verbose)
{
    return to_endpoint(address, true, verbose).hostname();
}
