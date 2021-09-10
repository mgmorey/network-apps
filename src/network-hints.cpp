#include "network-hints.h"      // Hints, struct addrinfo

#include <cassert>      // assert()

Network::Hints::Hints(int t_family,
                      int t_socktype,
                      int t_protocol,
                      int t_flags) :
    m_flags(t_flags),
    m_family(t_family),
    m_socktype(t_socktype),
    m_protocol(m_family, t_protocol)
{
}

Network::Hints::Hints(const addrinfo& t_addrinfo) :
    m_flags(t_addrinfo.ai_flags),
    m_family(t_addrinfo.ai_family),
    m_socktype(t_addrinfo.ai_socktype),
    m_protocol(m_family, t_addrinfo.ai_protocol)
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
    return (static_cast<int>(m_protocol) <
            static_cast<int>(t_hints.m_protocol) ||
            static_cast<int>(m_socktype) <
            static_cast<int>(t_hints.m_socktype) ||
            static_cast<int>(m_family) <
            static_cast<int>(t_hints.m_family));
}

bool Network::Hints::operator>(const Hints& t_hints) const
{
    return (static_cast<int>(m_protocol) >
            static_cast<int>(t_hints.m_protocol) ||
            static_cast<int>(m_socktype) >
            static_cast<int>(t_hints.m_socktype) ||
            static_cast<int>(m_family) >
            static_cast<int>(t_hints.m_family));
}

bool Network::Hints::operator==(const Hints& t_hints) const
{
    return (static_cast<int>(m_protocol) ==
            static_cast<int>(t_hints.m_protocol) &&
            static_cast<int>(m_socktype) ==
            static_cast<int>(t_hints.m_socktype) &&
            static_cast<int>(m_family) ==
            static_cast<int>(t_hints.m_family));
}

Network::Hints::operator addrinfo() const
{
    addrinfo ai = {
        static_cast<int>(m_flags),
        static_cast<int>(m_family),
        static_cast<int>(m_socktype),
        static_cast<int>(m_protocol),
        0,
        nullptr,
        nullptr,
        nullptr
    };
    assert(ai.ai_flags == static_cast<int>(m_flags));
    assert(ai.ai_family == static_cast<int>(m_family));
    assert(ai.ai_socktype == static_cast<int>(m_socktype));
    assert(ai.ai_protocol == static_cast<int>(m_protocol));
    assert(ai.ai_addrlen == 0);
    assert(ai.ai_addr == nullptr);
    assert(ai.ai_canonname == nullptr);
    assert(ai.ai_next == nullptr);
    return ai;
}

int Network::Hints::flags() const
{
    return static_cast<int>(m_flags);
}

int Network::Hints::family() const
{
    return static_cast<int>(m_family);
}

int Network::Hints::socktype() const
{
    return static_cast<int>(m_socktype);
}

int Network::Hints::protocol() const
{
    return static_cast<int>(m_protocol);
}
