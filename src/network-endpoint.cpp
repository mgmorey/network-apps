#include "network-address.h"    // Address, Endpoint, EndpointResult,
                                // Result, operator<<(), to_endpoint()
#include "network-buffer.h"     // Buffer

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
    std::string error;
    Buffer host {NI_MAXHOST};
    Buffer service {NI_MAXSERV};
    const auto code {::getnameinfo(addr(), addrlen(),
                                   &host[0], host.size(),
                                   &service[0], service.size(),
                                   t_flags)};

    if (t_verbose) {
        std::cerr << "Calling getnameinfo("
                  << *this
                  << ", ...)"
                  << std::endl;
    }

    if (code != 0) {
        std::ostringstream oss;
        oss << "Call to getnameinfo("
            << *this
            << ", ...) returned "
            << code
            << " ("
            << ::gai_strerror(code)
            << ')';
        error = oss.str();
    }

    Result result {code, error};
    assert(result.result() ?
           result.string() != "" :
           result.string() == "");
    return EndpointResult(Endpoint(host, service), result);
}

Network::EndpointResult
Network::Address::to_endpoint(bool t_numeric, bool t_verbose) const
{
    const int flags {t_numeric ? NI_NUMERICHOST | NI_NUMERICSERV : 0};
    return to_endpoint(flags, t_verbose);
}
