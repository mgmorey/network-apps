#include "network/host.h"               // Host, Hostname, SockAddr,
                                        // addrinfo
#include "network/get-sockaddr.h"       // SockAddr, get_sockaddr()

Network::Host::Host(const addrinfo& t_addrinfo) :
    m_sock_addr(get_sockaddr(t_addrinfo)),
    m_canonname(t_addrinfo.ai_canonname)
{
}

auto Network::Host::operator=(const addrinfo& t_addrinfo) -> Network::Host&
{
    m_sock_addr = get_sockaddr(t_addrinfo);
    m_canonname = t_addrinfo.ai_canonname;
    return *this;
}

auto Network::Host::operator<(const Host& t_host) const -> bool
{
    return m_sock_addr < t_host.m_sock_addr;
}

auto Network::Host::operator>(const Host& t_host) const -> bool
{
    return m_sock_addr > t_host.m_sock_addr;
}

auto Network::Host::operator==(const Host& t_host) const -> bool
{
    return m_sock_addr == t_host.m_sock_addr;
}

auto Network::Host::address() const -> Network::SockAddr
{
    return m_sock_addr;
}

auto Network::Host::canonical_name() const -> Network::Hostname
{
    return m_canonname;
}

auto Network::Host::get_sockaddr(const addrinfo& ai) -> Network::SockAddr
{
    return (ai.ai_addr == nullptr ?
            Network::get_sockaddr() :
            Network::get_sockaddr(ai.ai_addr, ai.ai_addrlen));
}
