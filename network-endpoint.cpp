#include "network-endpoint.h"   // Endpoint

#ifdef _WIN32
#include <ws2tcpip.h>   // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#else
#include <netdb.h>      // NI_MAXHOST, NI_MAXSERV, NI_NUMERICHOST,
                        // gai_strerror(), getnameinfo()
#endif

#include <cassert>      // assert()
#include <sstream>      // std::ostringstream

Network::Endpoint::Endpoint(const Address& addr, int flags) :
    code(0),
    host(NI_MAXHOST),
    serv(NI_MAXSERV)
{
    assert(addr.size());
    assert(host.size());
    assert(serv.size());
    code = ::getnameinfo(addr.data(), addr.size(),
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

Network::Endpoint Network::to_endpoint(const Address& other, bool numeric)
{
    int flags = numeric ? NI_NUMERICHOST | NI_NUMERICSERV : 0;
    return Endpoint(other, flags);
}

Network::Hostname Network::to_hostname(const Address& other, bool numeric)
{
    return to_endpoint(other, numeric).hostname();
}

Network::Hostname Network::to_string(const Address& other)
{
    return to_hostname(other, true);
}
