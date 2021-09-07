#include "network-host.h"       // Address, Host, Nullable, struct
                                // addrinfo

Network::Host::Host(const addrinfo& t_addrinfo) :
    m_address(t_addrinfo.ai_addr, t_addrinfo.ai_addrlen),
    m_canonname(t_addrinfo.ai_canonname)
{
}

Network::Host::Host()
{
}

bool Network::Host::operator<(const Host& t_host) const
{
    return m_address < t_host.m_address;
}

bool Network::Host::operator>(const Host& t_host) const
{
    return m_address > t_host.m_address;
}

bool Network::Host::operator==(const Host& t_host) const
{
    return m_address == t_host.m_address;
}

Network::Address Network::Host::address() const
{
    return m_address;
}

Network::Nullable Network::Host::canonical_name() const
{
    return m_canonname;
}
