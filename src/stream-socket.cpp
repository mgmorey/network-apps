#include "network-socket.h"     // Socket
#include "network-address.h"    // Address, operator<<()
#include "network-family.h"     // Family, operator<<()
#include "network-flags.h"      // Flags, operator<<()
#include "network-format.h"     // Format, operator<<()
#include "network-nullable.h"   // Nullable, operator<<()
#include "network-protocol.h"   // Protocol, operator<<()
#include "network-socktype.h"   // SockType, operator<<()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const Socket& sock)
{
    static const int tabs[1] = {7};

    const auto flags = sock.m_flags;
    const auto family = sock.m_family;
    const auto socktype = sock.m_socktype;
    const auto protocol = sock.m_protocol;

    os << "Socket("
       << Format("m_flags")
       << Flags(flags)
       << Format(tabs[0], "m_family")
       << Family(family)
       << Format(tabs[0], "m_socktype")
       << SockType(socktype)
       << Format(tabs[0], "m_protocol")
       << Protocol(family, protocol)
       << Format(tabs[0], "m_address")
       << sock.m_address
       << Format(tabs[0], "m_canonname")
       << sock.m_canonname
       << ')';
    return os;
}
