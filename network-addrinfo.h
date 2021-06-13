#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, freeaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, freeaddrinfo()
#endif

namespace Network
{
    extern struct addrinfo* get_addrinfo(std::string& error,
                                         const Hostname& node,
                                         const Service& service,
                                         const struct addrinfo* hints);

    template <class Container>
    void copy_addrinfo(Container& dest,
                       std::string& error,
                       const Hostname& node,
                       const Service& service,
                       const struct addrinfo* hints)
    {
        struct addrinfo* list = get_addrinfo(error, node, service, hints);

        if (list != NULL) {
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
