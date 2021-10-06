#include "network-host.h"       // Address, Host, Nullable, addrinfo
#include "network-types.h"      // SockAddr

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

Network::Nullable Network::Host::canonical_name() const
{
    return m_canonname;
}

Network::SockAddr Network::Host::get_sockaddr(const addrinfo& ai)
{
    auto data {reinterpret_cast<const std::byte*>(ai.ai_addr)};
    auto size {static_cast<std::size_t>(ai.ai_addrlen)};
    return SockAddr(data, data + size);
}
