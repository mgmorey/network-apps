#include "network-hints.h"      // Hints, struct addrinfo

#include <cassert>      // assert()

Network::Hints::Hints(int t_family,
                        int t_socktype,
                        int t_protocol,
                        int t_flags) :
    m_flags(t_flags),
    m_family(t_family),
    m_socktype(t_socktype),
    m_protocol(t_protocol)
{
}

Network::Hints::Hints(const addrinfo& t_addrinfo) :
    m_flags(t_addrinfo.ai_flags),
    m_family(t_addrinfo.ai_family),
    m_socktype(t_addrinfo.ai_socktype),
    m_protocol(t_addrinfo.ai_protocol)
{
}

Network::Hints& Network::Hints::operator=(const addrinfo& t_addrinfo)
{
    m_flags = t_addrinfo.ai_flags;
    m_family = t_addrinfo.ai_family;
    m_socktype = t_addrinfo.ai_socktype;
    m_protocol = t_addrinfo.ai_protocol;
    return *this;
}

bool Network::Hints::operator<(const Hints& t_hints) const
{
    return (m_protocol < t_hints.m_protocol ||
            m_socktype < t_hints.m_socktype ||
            m_family < t_hints.m_family);
}

bool Network::Hints::operator>(const Hints& t_hints) const
{
    return (m_protocol > t_hints.m_protocol ||
            m_socktype > t_hints.m_socktype ||
            m_family > t_hints.m_family);
}

bool Network::Hints::operator==(const Hints& t_hints) const
{
    return (m_protocol == t_hints.m_protocol &&
            m_socktype == t_hints.m_socktype &&
            m_family == t_hints.m_family);
}

Network::Hints::operator addrinfo() const
{
    addrinfo ai = {
        m_flags,
        m_family,
        m_socktype,
        m_protocol,
        0,
        nullptr,
        nullptr,
        nullptr
    };
    assert(ai.ai_flags == m_flags);
    assert(ai.ai_family == m_family);
    assert(ai.ai_socktype == m_socktype);
    assert(ai.ai_protocol == m_protocol);
    assert(ai.ai_addrlen == 0);
    assert(ai.ai_addr == nullptr);
    assert(ai.ai_canonname == nullptr);
    assert(ai.ai_next == nullptr);
    return ai;
}

int Network::Hints::flags() const
{
    return m_flags;
}

int Network::Hints::family() const
{
    return m_family;
}

int Network::Hints::socktype() const
{
    return m_socktype;
}

int Network::Hints::protocol() const
{
    return m_protocol;
}
