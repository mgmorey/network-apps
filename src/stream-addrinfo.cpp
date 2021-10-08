#include "network-addrinfo.h"   // operator<<()
#include "network-address.h"    // Address, operator<<()
#include "network-family.h"     // Family, operator<<()
#include "network-flags.h"      // Flags, operator<<()
#include "network-format.h"     // Format, operator<<()
#include "network-optional.h"   // Optional, operator<<()
#include "network-protocol.h"   // Protocol, operator<<()
#include "network-sockaddr.h"   // get_sockaddr()
#include "network-socktype.h"   // SockType, operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const addrinfo& ai)
{
    static const int tab {9};

    const Flags flags {ai.ai_flags};
    const Family family {ai.ai_family};
    const SockType socktype {ai.ai_socktype};
    const Protocol protocol {family, ai.ai_protocol};

    os << "addrinfo("
       << Format("ai_flags")
       << flags
       << Format(tab, "ai_family")
       << family
       << Format(tab, "ai_socktype")
       << socktype
       << Format(tab, "ai_protocol")
       << protocol
       << Format(tab, "ai_addrlen")
       << ai.ai_addrlen
       << Format(tab, "ai_addr")
       << Address(get_sockaddr(ai.ai_addr, ai.ai_addrlen))
       << Format(tab, "ai_canonname")
       << Optional(ai.ai_canonname)
       << Format(tab)
       << "...)";
    return os;
}
