#include "network-address.h"    // Address, operator<<()
#include "network-addrinfo.h"   // operator<<()
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
    static const int tabs[1] = {9};

    const auto flags = ai.ai_flags;
    const auto family = ai.ai_family;
    const auto socktype = ai.ai_socktype;
    const auto protocol = ai.ai_protocol;

    os << "addrinfo("
       << Format("ai_flags")
       << Flags(flags)
       << Format(tabs[0], "ai_family")
       << Family(family)
       << Format(tabs[0], "ai_socktype")
       << SockType(socktype)
       << Format(tabs[0], "ai_protocol")
       << Protocol(family, protocol)
       << Format(tabs[0], "ai_addrlen")
       << ai.ai_addrlen
       << Format(tabs[0], "ai_addr")
       << Address(ai.ai_addr, ai.ai_addrlen)
       << Format(tabs[0], "ai_canonname")
       << Nullable(ai.ai_canonname)
       << Format(tabs[0], "ai_next")
       << ai.ai_next
       << ')';
    return os;
}
