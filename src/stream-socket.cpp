#include "network-socket.h"     // Socket, operator<<()
#include "network-format.h"     // Format, operator<<()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const Socket& sock)
{
    static const int tabs[1] = {7};

    os << "Socket("
       << Format("m_flags")
       << sock.m_flags
       << Format(tabs[0], "m_family")
       << sock.m_family
       << Format(tabs[0], "m_socktype")
       << sock.m_socktype
       << Format(tabs[0], "m_protocol")
       << sock.m_protocol
       << Format(tabs[0], "m_address")
       << sock.m_address
       << Format(tabs[0], "m_canonname")
       << sock.m_canonname
       << ')';
    return os;
}
