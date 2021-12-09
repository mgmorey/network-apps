#include "network/hints.h"      // Hints, addrinfo

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
        0,
        0,
        0,
        0,
        0,
        nullptr,
        nullptr,
        nullptr
    };
    ai.ai_flags = static_cast<int>(m_flags);
    ai.ai_family = static_cast<int>(m_family);
    ai.ai_socktype = static_cast<int>(m_socktype);
    ai.ai_protocol = static_cast<int>(m_protocol);
    return ai;
}

Network::Flags Network::Hints::flags() const
{
    return Flags(m_flags);
}

Network::Family Network::Hints::family() const
{
    return Family(m_family);
}

Network::SockType Network::Hints::socktype() const
{
    return SockType(m_socktype);
}

Network::Protocol Network::Hints::protocol() const
{
    return {m_family, m_protocol};
}
