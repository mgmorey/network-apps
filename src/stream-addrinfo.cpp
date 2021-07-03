#include "stream-addrinfo.h"    // struct addrinfo, std::ostream,
                                // operator<<()
#include "network-family.h"     // Family, operator<<()
#include "network-flags.h"      // Flags, operator<<()
#include "network-format.h"     // Format, operator<<()
#include "network-name.h"       // Name, operator<<()
#include "network-protocol.h"   // Protocol, operator<<()
#include "network-socktype.h"   // SockType, operator<<()
#include "stream-address.h"     // Address, operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

std::ostream& Network::operator<<(std::ostream& os,
                                  const addrinfo& ai)
{
    static const int tabs[1] = {9};

    const int flags = ai.ai_flags;
    const int family = ai.ai_family;
    const int socktype = ai.ai_socktype;
    const int protocol = ai.ai_protocol;

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
       << Name(ai.ai_canonname)
       << Format(tabs[0], "ai_next")
       << ai.ai_next
       << ')';
    return os;
}
