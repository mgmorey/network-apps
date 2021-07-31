#include "network-address.h"    // Address, Endpoint, EndpointResult,
                                // Hostname, Result, to_endpoint()
#include "network-buffer.h"     // Buffer
#include "stream-address.h"     // operator<<()

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

Network::EndpointResult
Network::Address::to_endpoint(int flags, bool verbose) const
{
    assert(!empty());
    std::string error;
    Buffer host_buffer(NI_MAXHOST);
    Buffer service_buffer(NI_MAXSERV);
    const int code = ::getnameinfo(addr(), addrlen(),
                                   &host_buffer[0], host_buffer.size(),
                                   &service_buffer[0], service_buffer.size(),
                                   flags);

    if (verbose) {
        std::cerr << "Invoking getnameinfo("
                  << *this
                  << ", ...)"
                  << std::endl;
    }

    if (code != 0) {
        std::ostringstream oss;
        oss << "getnameinfo("
            << *this
            << ", ...) returned "
            << code
            << " ("
            << ::gai_strerror(code)
            << ')';
        error = oss.str();
    }

    return EndpointResult(Endpoint(host_buffer, service_buffer), Result(code, error));
}

Network::EndpointResult
Network::Address::to_endpoint(bool numeric, bool verbose) const
{
    const int flags = numeric ? NI_NUMERICHOST | NI_NUMERICSERV : 0;
    return to_endpoint(flags, verbose);
}
