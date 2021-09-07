#include "network-hints.h"      // Hints, struct addrinfo

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
    return ai;
}

int Network::Hints::family() const
{
    return m_family;
}
