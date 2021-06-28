#include "network-endpoint.h"   // Endpoint, EndpointResult, Hostname,
                                // Result, SockAddr, to_endpoint(),
                                // to_hostname(), to_string()
#include "network-buffer.h"     // Buffer

#ifdef _WIN32
#include <ws2tcpip.h>   // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#else
#include <netdb.h>      // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::EndpointResult Network::to_endpoint(const Network::SockAddr& address,
                                             int flags, bool verbose)
{
    std::string error;
    Buffer host_buffer(NI_MAXHOST);
    Buffer service_buffer(NI_MAXSERV);
    int code = ::getnameinfo(address, address.size(),
                             &host_buffer[0], host_buffer.size(),
                             &service_buffer[0], service_buffer.size(),
                             flags);

    if (verbose) {
        std::cerr << "Invoking getnameinfo("
                  << address
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

    return EndpointResult(Endpoint(host_buffer,
                                   service_buffer),
                          Result(code, error));
}

Network::EndpointResult Network::to_endpoint(const SockAddr& address,
                                             bool numeric,
                                             bool verbose)
{
    int flags = numeric ? NI_NUMERICHOST | NI_NUMERICSERV : 0;
    return to_endpoint(address, flags, verbose);
}

Network::Hostname Network::to_hostname(const SockAddr& address,
                                       bool verbose)
{
    return to_endpoint(address, false, verbose).first.first;
}

Network::Hostname Network::to_string(const SockAddr& address,
                                     bool verbose)
{
    return to_endpoint(address, true, verbose).first.first;
}
