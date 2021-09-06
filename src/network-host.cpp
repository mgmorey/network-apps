#include "network-host.h"       // Address, Host, struct addrinfo

Network::Host::Host(const addrinfo& t_addrinfo) :
    m_canonname(t_addrinfo.ai_canonname),
    m_address(t_addrinfo.ai_addr, t_addrinfo.ai_addrlen)
{
}

Network::Host::Host(const Socket& t_socket) :
    m_canonname(t_socket.m_canonname),
    m_address(t_socket.m_address)
{
}

Network::Host& Network::Host::operator=(const Socket& t_socket)
{
    m_canonname = t_socket.m_canonname;
    m_address = t_socket.m_address;
    return *this;
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

Network::Host::operator Address() const
{
    return m_address;
}

Network::Nullable Network::Host::canonical_name() const
{
    return m_canonname;
}
