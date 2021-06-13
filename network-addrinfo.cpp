#include "network-addrinfo.h"   // Hostname, Service, Result, struct
                                // addrinfo, get_addrinfo()

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, getaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, getaddrinfo()
#endif

#include <cassert>      // assert()
#include <sstream>      // std::ostringstream

struct addrinfo* Network::get_addrinfo(Result& result,
                                       const Hostname& node,
                                       const Service& service,
                                       const struct addrinfo* hints)
{
    struct addrinfo* list = NULL;
    assert(!node.empty() || !service.empty());
    result.first = ::getaddrinfo(node.empty() ? NULL : node.c_str(),
                                 service.empty() ? NULL : service.c_str(),
                                 hints,
                                 &list);

    if (result.first != 0) {
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
           << result.first
           << " ("
           << ::gai_strerror(result.first)
           << ')';

        result.second = os.str();
    }
    else {
        result.second.clear();
        assert(list != NULL);
    }

    return list;
}
