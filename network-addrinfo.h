#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, freeaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, freeaddrinfo()
#endif

namespace Network
{
    extern struct addrinfo* get_addrinfo(Result& result,
                                         const Hostname& node,
                                         const Service& service,
                                         const struct addrinfo* hints);

    template <class Container>
    void copy_addrinfo(Container& dest,
                       const Hostname& node,
                       const Service& service,
                       const struct addrinfo* hints)
    {
        struct addrinfo* list = get_addrinfo(dest.second, node, service, hints);

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
