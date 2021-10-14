#include "network-host.h"       // Address, Host, addrinfo
#include "network-sockaddr.h"   // SockAddr, is_valid()

#include <cassert>      // assert()

Network::Host::Host()
{
}

Network::Host::Host(const addrinfo& t_addrinfo) :
    m_address(get_sockaddr(t_addrinfo)),
    m_canonname(t_addrinfo.ai_canonname)
{
}

Network::Host& Network::Host::operator=(const addrinfo& t_addrinfo)
{
    m_address = get_sockaddr(t_addrinfo);
    m_canonname = t_addrinfo.ai_canonname;
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

Network::Address Network::Host::address() const
{
    return m_address;
}

Network::Hostname Network::Host::canonical_name() const
{
    return m_canonname;
}

Network::SockAddr Network::Host::get_sockaddr(const addrinfo& ai)
{
    const auto addr_data {reinterpret_cast<const Byte*>(ai.ai_addr)};
    const auto addr_size {static_cast<std::size_t>(ai.ai_addrlen)};
    return SockAddr(addr_data, addr_data + addr_size);
}
