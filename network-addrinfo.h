#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, freeaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, freeaddrinfo()
#endif

#include <iomanip>      // std::setw()
#include <iostream>     // std::cerr, std::endl
#include <ostream>      // std::endl, std::ostream
#include <utility>      // std::pair
#include <vector>       // std::vector

namespace Network
{
    typedef std::pair<struct addrinfo*, Result> AddrinfoResult;

    extern std::ostream& operator<<(std::ostream& os,
                                    const struct addrinfo& ai);
    extern AddrinfoResult get_addrinfo(const Hostname& node,
                                       const Service& service,
                                       const struct addrinfo* hints);
    extern void print_statistics(std::vector<std::size_t> sizes);

    template <class Container>
    void copy_addrinfo(Container& dest,
                       const Hostname& node,
                       const Service& service,
                       const struct addrinfo* hints,
                       bool verbose)
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
            if (verbose) {
                std::cerr << "Fetched addrinfo "
                          << elem
                          << ':'
                          << std::endl
                          << *elem
                          << std::endl;
            }
            dest.first.push_back(*elem);
        }

        ::freeaddrinfo(list);
    }

    template <class Container>
    Container get_addrinfo(const Hostname& node,
                           const Service& service,
                           const struct addrinfo* hints,
                           bool verbose)
    {
        Container container;
        std::vector<std::size_t> sizes;
        copy_addrinfo(container, node, service, hints, verbose);
        sizes.push_back(container.first.size());
        container.first.unique();
        sizes.push_back(container.first.size());

        if (verbose) {
            print_statistics(sizes);
        }

        return container;
    }
}

#endif
