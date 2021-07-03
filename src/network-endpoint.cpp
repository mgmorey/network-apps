#include "network-endpoint.h"   // Address, Endpoint, EndpointResult,
                                // Hostname, Result, to_endpoint(),
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

Network::EndpointResult Network::to_endpoint(const Address& address,
                                             int flags, bool verbose)
{
    std::string error;
    Buffer host_buffer(NI_MAXHOST);
    Buffer service_buffer(NI_MAXSERV);
    const int code = ::getnameinfo(address, address.size(),
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
        os << "getnameinfo("
           << address
           << ", ...) returned "
           << code
           << " ("
           << ::gai_strerror(code)
           << ')';
        error = os.str();
    }

    return EndpointResult(Endpoint(host_buffer, service_buffer), Result(code, error));
}

Network::EndpointResult Network::to_endpoint(const Address& address,
                                             bool numeric,
                                             bool verbose)
{
    const int flags = numeric ? NI_NUMERICHOST | NI_NUMERICSERV : 0;
    return to_endpoint(address, flags, verbose);
}
