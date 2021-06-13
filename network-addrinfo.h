#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, freeaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, freeaddrinfo()
#endif

#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<struct addrinfo*, Result> AddrinfoResult;

    extern AddrinfoResult get_addrinfo(const Hostname& node,
                                       const Service& service,
                                       const struct addrinfo* hints);

    template <class Container>
    void copy_addrinfo(Container& dest,
                       const Hostname& node,
                       const Service& service,
                       const struct addrinfo* hints)
    {
        AddrinfoResult result(get_addrinfo(node, service, hints));
        struct addrinfo* list = result.first;
        dest.second = result.second;

        if (list == NULL) {
            return;
        }

        for (const struct addrinfo* elem = list;
             elem != NULL;
             elem = elem->ai_next) {
            dest.first.push_back(*elem);
        }

        ::freeaddrinfo(list);
    }

    template <class Container>
    Container get_addrinfo(const Hostname& node,
                           const Service& service,
                           const struct addrinfo* hints)
    {
        Container container;
        copy_addrinfo(container, node, service, hints);
        container.first.unique();
        return container;
    }
}

#endif
