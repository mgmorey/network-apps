#include "network-addrinfo.h"   // AddrinfoResult, Hostname, Service,
                                // Result, struct addrinfo,
                                // get_addrinfo(), std::cerr,
                                // std::endl, std::ostream,
                                // std::string
#include "network-format.h"     // Format
#include "network-string.h"     // to_string()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP,
                        // SOCK_DGRAM, SOCK_STREAM
#include <ws2tcpip.h>   // struct addrinfo, getaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, getaddrinfo()
#include <netinet/in.h> // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP
#include <sys/socket.h> // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // SOCK_DGRAM, SOCK_STREAM
#endif

#include <cassert>      // assert()
#include <iomanip>	// std::hex
#include <sstream>      // std::ostringstream

Network::Family::Family(int family) :
    value(family)
{
}

Network::Flags::Flags(int flags) :
    value(flags)
{
}

Network::Name::Name(const std::string &name) :
    null(false),
    value(name)
{
}

Network::Name::Name(const char* name) :
    null(name == NULL),
    value(null ? "" : name)
{
}

Network::Protocol::Protocol(int protocol) :
    value(protocol)
{
}

Network::SockType::SockType(int socktype) :
    value(socktype)
{
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const struct addrinfo& ai)
{
    static const int tabs[1] = {9};
    os << "addrinfo("
       << Format("ai_flags")
       << Flags(ai.ai_flags)
       << Format(tabs[0], "ai_family")
       << Family(ai.ai_family)
       << Format(tabs[0], "ai_socktype")
       << SockType(ai.ai_socktype)
       << Format(tabs[0], "ai_protocol")
       << Protocol(ai.ai_protocol)
       << Format(tabs[0], "ai_addrlen")
       << ai.ai_addrlen
       << Format(tabs[0], "ai_addr")
       << to_string(ai.ai_addr, ai.ai_addrlen)
       << Format(tabs[0], "ai_canonname")
       << Name(ai.ai_canonname)
       << Format(tabs[0], "ai_next")
       << ai.ai_next
       << ')';
    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Family& family)
{
    switch(family.value) {
    case AF_INET:
        os << "AF_INET";
        break;
    case AF_INET6:
        os << "AF_INET6";
        break;
    case AF_UNSPEC:
        os << "AF_UNSPEC";
        break;
    default:
        os << family.value;
    }

    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Flags& flags)
{
    static struct values {
        int flag;
        const char* label;
    } values[] = {
        {AI_PASSIVE,                "AI_PASSIVE"},
        {AI_CANONNAME,              "AI_CANONNAME"},
        {AI_NUMERICHOST,            "AI_NUMERICHOST"},
        {AI_NUMERICSERV,            "AI_NUMERICSERV"},
        {AI_ALL,                    "AI_ALL"},
        {AI_ADDRCONFIG,             "AI_ADDRCONFIG"},
        {AI_V4MAPPED,               "AI_V4MAPPED"},
        {AI_NON_AUTHORITATIVE,      "AI_NON_AUTHORITATIVE"},
        {AI_SECURE,                 "AI_SECURE"},
        {AI_RETURN_PREFERRED_NAMES, "AI_RETURN_PREFERRED_NAMES"},
        {0,                         NULL}
    };
    std::ostringstream oss;
    std::size_t n = 0;

    for(const struct values* p = values; p->flag; ++p) {
        if (flags.value & p->flag) {
            if (n) {
                oss << " | ";
            }

            oss << p->label;
            ++n;
        }
    }

    if (n > 1) {
        os << '(' << oss.str() << ')';
    }
    else if (n > 0) {
        os << oss.str();
    }
    else {
        os << 0;
    }

    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Name& name)
{
    if (name.null) {
        os << std::hex << 0;
    }
    else {
        os << '"'
           << name.value
           << '"';
    }

    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Protocol& protocol)
{
    switch(protocol.value) {
    case IPPROTO_IP:
        os << "IPPROTO_IP";
        break;
    case IPPROTO_TCP:
        os << "IPPROTO_TCP";
        break;
    case IPPROTO_UDP:
        os << "IPPROTO_UDP";
        break;
    default:
        os << protocol.value;
    }

    return os;
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const SockType& socktype)
{
    switch(socktype.value) {
    case SOCK_DGRAM:
        os << "SOCK_DGRAM";
        break;
    case SOCK_STREAM:
        os << "SOCK_STREAM";
        break;
    default:
        os << socktype.value;
    }

    return os;
}

Network::AddrinfoResult Network::get_addrinfo(const Hostname& node,
                                              const Service& service,
                                              const struct addrinfo* hints,
                                              bool verbose)
{
    std::string error;
    struct addrinfo* list = NULL;
    assert(!node.empty() || !service.empty());

    if (verbose) {
        std::cerr << "Invoking getaddrinfo("
                  << Name(node)
                  << ", "
                  << Name(service)
                  << ", "
                  << hints
                  << ", ...)"
                  << std::endl
                  << "Using hints (addrinfo) "
                  << hints
                  << ':'
                  << std::endl
                  << *hints
                  << std::endl;
    }

    int code = ::getaddrinfo(node.empty() ? NULL : node.c_str(),
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

    return AddrinfoResult(list, Result(code, error));
}

void Network::print_statistics(const std::vector<std::size_t>& sizes)
{
    assert(sizes.size() == 2);
    assert(sizes[0] >= sizes[1]);

    if (sizes[0]) {
        std::cerr << "Fetched "
                  << sizes[0]
                  << " items";
        std::size_t delta = sizes[0] - sizes[1];

        if (delta) {
            std::cerr << " ("
                      << sizes[1]
                      << " unique + "
                      << delta
                      << " duplicate)";
        }

        std::cerr << std::endl;
    }
}
