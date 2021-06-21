#include "network-addrinfo.h"   // AddrinfoResult, Hostname, Service,
                                // Result, struct addrinfo,
                                // get_addrinfo(), std::ostream,
                                // std::string
#include "network-format.h"     // Format
#include "network-string.h"     // to_string()

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, getaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, getaddrinfo()
#endif

#include <cassert>      // assert()
#include <sstream>      // std::ostringstream

std::ostream& Network::operator<<(std::ostream& os,
                                  const struct addrinfo& ai)
{
    static const int tabs[1] = {9};
    os << "addrinfo("
       << Format("ai_flags")
       << ai.ai_flags
       << Format(tabs[0], "ai_family")
       << ai.ai_family
       << Format(tabs[0], "ai_socktype")
       << ai.ai_socktype
       << Format(tabs[0], "ai_protocol")
       << ai.ai_protocol
       << Format(tabs[0], "ai_addrlen")
       << ai.ai_addrlen
       << Format(tabs[0], "ai_addr")
       << to_string(ai.ai_addr, ai.ai_addrlen)
       << Format(tabs[0], "ai_canonname")
       << (ai.ai_canonname != NULL ?
           ai.ai_canonname :
           "NULL")
       << Format(tabs[0], "ai_next")
       << ai.ai_next
       << ')';
    return os;
}

Network::AddrinfoResult Network::get_addrinfo(const Hostname& node,
                                              const Service& service,
                                              const struct addrinfo* hints)
{
    std::string error;
    struct addrinfo* list = NULL;
    assert(!node.empty() || !service.empty());
    int code = ::getaddrinfo(node.empty() ? NULL : node.c_str(),
                             service.empty() ? NULL : service.c_str(),
                             hints,
                             &list);

    if (code != 0) {
        std::ostringstream os;
        os << "getaddrinfo(";

        if (!node.empty()) {
            os << '"'
               << node
               << '"';
        }
        else {
            os << "NULL";
        }

        os << ", ";

        if (!service.empty()) {
            os << '"'
               << service
               << "\", ";
        }

        os << "...) returned "
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
