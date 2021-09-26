#include "network-socket.h"     // Socket, operator<<()
#include "network-format.h"     // Format, operator<<()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const Socket& sock)
{
    static const int tab {7};

    os << "Socket("
       << Format("m_flags")
       << sock.m_flags
       << Format(tab, "m_family")
       << sock.m_family
       << Format(tab, "m_socktype")
       << sock.m_socktype
       << Format(tab, "m_protocol")
       << sock.m_protocol
       << Format(tab, "m_address")
       << sock.m_address
       << Format(tab, "m_canonname")
       << sock.m_canonname
       << ')';
    return os;
}
