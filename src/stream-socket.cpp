#include "network/socket.h"     // Socket, operator<<()
#include "network/format.h"     // Format, operator<<()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const Socket& sock)
{
    static const int tab {7};

    os << "Socket("
       << Format("flags")
       << sock.flags()
       << Format(tab, "family")
       << sock.family()
       << Format(tab, "socktype")
       << sock.socktype()
       << Format(tab, "protocol")
       << sock.protocol()
       << Format(tab, "address")
       << sock.address()
       << Format(tab, "canonical_name")
       << sock.canonical_name()
       << ')';
    return os;
}
