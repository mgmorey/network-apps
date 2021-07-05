#include "network-host.h"       // Address, Host, Result

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

Network::Host::Host(const addrinfo& other) :
    address(other.ai_addr, other.ai_addrlen),
    cname(other.ai_canonname)
{
}

Network::Host& Network::Host::operator=(const addrinfo& other)
{
    address = Address(other.ai_addr, other.ai_addrlen);
    cname = other.ai_canonname;
    return *this;
}

bool Network::Host::operator<(const Host& other) const
{
    return address < other.address;
}

bool Network::Host::operator>(const Host& other) const
{
    return address > other.address;
}

bool Network::Host::operator==(const Host& other) const
{
    return address == other.address;
}

Network::Host::operator Address() const
{
    return address;
}

Network::Nullable Network::Host::canonical_name() const
{
    return cname;
}
