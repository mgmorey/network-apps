#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, freeaddrinfo(),
                        // getaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, freeaddrinfo(),
                        // getaddrinfo()
#endif

#include <cassert>      // assert()
#include <sstream>      // std::ostringstream

namespace Network
{
    template <class Container>
    void copy_addrinfo(Container& dest,
                       std::string& error,
                       const Hostname& node,
                       const Service& service,
                       const struct addrinfo* hints)
    {
        struct addrinfo* list = NULL;
        assert(!node.empty() || !service.empty());
        int result = ::getaddrinfo(node.empty() ? NULL : node.c_str(),
                                   service.empty() ? NULL : service.c_str(),
                                   hints,
                                   &list);

        if (result != 0) {
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
               << result
               << " ("
               << ::gai_strerror(result)
               << ')';

            error = os.str();
        }
        else {
            assert(list != NULL);

            for (const struct addrinfo* elem = list;
                 elem != NULL;
                 elem = elem->ai_next) {
                dest.push_back(*elem);
            }

            ::freeaddrinfo(list);
        }
    }

    template <class Container>
    Container get_addrinfo(const Hostname& node,
                           const Service& service,
                           const struct addrinfo* hints)
    {
        Container container;
        copy_addrinfo(container.first, container.second, node, service, hints);
        container.first.unique();
        return container;
    }
}

#endif
