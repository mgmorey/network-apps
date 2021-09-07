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
