#include "network-addrinfo.h"   // AddrinfoResult, Hostname, Service,
                                // Result, struct addrinfo,
                                // get_addrinfo()

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, getaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, getaddrinfo()
#endif

#include <cassert>      // assert()
#include <sstream>      // std::ostringstream

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
