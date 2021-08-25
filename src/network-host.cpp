#include "network-host.h"       // Address, Host, Result

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

Network::Host::Host(const addrinfo& t_addrinfo) :
    m_address(t_addrinfo.ai_addr, t_addrinfo.ai_addrlen),
    m_cname(t_addrinfo.ai_canonname)
{
}

Network::Host& Network::Host::operator=(const addrinfo& t_addrinfo)
{
    m_address = Address(t_addrinfo.ai_addr, t_addrinfo.ai_addrlen);
    m_cname = t_addrinfo.ai_canonname;
    return *this;
}

bool Network::Host::operator<(const Host& t_addrinfo) const
{
    return m_address < t_addrinfo.m_address;
}

bool Network::Host::operator>(const Host& t_addrinfo) const
{
    return m_address > t_addrinfo.m_address;
}

bool Network::Host::operator==(const Host& t_addrinfo) const
{
    return m_address == t_addrinfo.m_address;
}

Network::Host::operator Address() const
{
    return m_address;
}

Network::Nullable Network::Host::canonical_name() const
{
    return m_cname;
}
