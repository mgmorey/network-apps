#include "network-addrinfo.h"   // operator<<()
#include "network-address.h"    // Address, operator<<()
#include "network-family.h"     // Family, operator<<()
#include "network-flags.h"      // Flags, operator<<()
#include "network-format.h"     // Format, operator<<()
#include "network-nullable.h"   // Nullable, operator<<()
#include "network-protocol.h"   // Protocol, operator<<()
#include "network-socktype.h"   // SockType, operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
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
       << Address(ai.ai_addr, ai.ai_addrlen)
       << Format(tab, "ai_canonname")
       << Nullable(ai.ai_canonname)
       << Format(tab, "ai_next")
       << ai.ai_next
       << ')';
    return os;
}
