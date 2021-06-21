#include "network-endpoint.h"   // Address, Endpoint, Result,
                                // to_string()

#ifdef _WIN32
#include <ws2tcpip.h>   // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#else
#include <netdb.h>      // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#endif

#include <cassert>      // assert()
#include <sstream>      // std::ostringstream

Network::Endpoint::Endpoint(const Address& address, int flags) :
    code(0),
    host(NI_MAXHOST),
    serv(NI_MAXSERV)
{
    assert(address.size());
    code = ::getnameinfo(address.data(), address.size(),
                         &host[0], host.size(),
                         &serv[0], serv.size(),
                         flags);

    if (code != 0) {
        std::ostringstream os;
        os << "getnameinfo() returned "
           << code
           << " ("
           << ::gai_strerror(code)
           << ')';
        error = os.str();
    }
}

Network::Hostname Network::Endpoint::hostname() const
{
    return host.data();
}

Network::Service Network::Endpoint::service() const
{
    return serv.data();
}

Network::Result Network::Endpoint::result() const
{
    return Result(code, error);
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Address& address)
{
    return os << to_string(address);
}

Network::Endpoint Network::to_endpoint(const Address& address,
                                       bool numeric)
{
    int flags = numeric ? NI_NUMERICHOST | NI_NUMERICSERV : 0;
    return Endpoint(address, flags);
}

Network::Hostname Network::to_hostname(const Address& address,
                                       bool numeric)
{
    return to_endpoint(address, numeric).hostname();
}

Network::Hostname Network::to_string(const Address& address)
{
    return to_hostname(address, true);
}
