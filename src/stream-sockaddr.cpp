#include "network-sockaddr.h"   // operator<<()
#include "network-tostring.h"   // to_string()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const SockAddr& sock_addr)
{
    os << to_string(sock_addr, true);
    return os;
}
