#include "network-addrinfo.h"   // AddrinfoResult, Hostname, Service,
                                // Result, struct addrinfo,
                                // copy_addrinfo(), get_addrinfo(),
                                // std::cerr, std::endl, std::ostream,
                                // std::size_t, std::string
#include "network-address.h"    // Address, operator<<()
#include "network-family.h"     // Family, operator<<()
#include "network-flags.h"      // Flags, operator<<()
#include "network-format.h"     // Format, operator<<()
#include "network-name.h"       // Name, operator<<()
#include "network-protocol.h"   // Protocol, operator<<()
#include "network-socktype.h"   // SockType, operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, getaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, getaddrinfo()
#endif

#include <cassert>      // assert()
#include <ostream>      // std::ostream
#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::AiResult Network::get_addrinfo(const Hostname& node,
                                        const Service& service,
                                        const addrinfo* hints,
                                        bool verbose)
{
    if (verbose) {
        std::cerr << "Invoking getaddrinfo("
                  << Name(node)
                  << ", "
                  << Name(service)
                  << ", "
                  << hints
                  << ", ...)"
                  << std::endl
                  << "Using addrinfo hints "
                  << hints
                  << ':'
                  << std::endl
                  << *hints
                  << std::endl;
    }

    std::string error;
    addrinfo* list = NULL;
    assert(!node.empty() || !service.empty());
    const int code = ::getaddrinfo(node.empty() ? NULL : node.c_str(),
                                   service.empty() ? NULL : service.c_str(),
                                   hints,
                                   &list);

    if (code != 0) {
        std::ostringstream os;
        os << "getaddrinfo("
           << Name(node)
           << ", "
           << Name(service)
           << ", "
           << hints
           << ") returned "
           << code
           << " ("
           << ::gai_strerror(code)
           << ')';
        error = os.str();
    }
    else {
        assert(list != NULL);
    }

    return AiResult(list, Result(code, error));
}

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
