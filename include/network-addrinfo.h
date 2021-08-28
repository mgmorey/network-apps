#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service
#include "network-unique.h"     // Unique, operator<<()
#include "stream-addrinfo.h"    // operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, freeaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, freeaddrinfo()
#endif

#include <cstddef>      // std::size_t
#include <iostream>     // std::cerr, std::endl
#include <string>       // std::string
#include <utility>      // std::pair
#include <vector>       // std::vector

namespace Network
{
    typedef std::pair<addrinfo*, Result> AiResult;

    extern AiResult get_addrinfo(const Hostname& node,
                                 const Service& service,
                                 const addrinfo* hints,
                                 bool verbose);

    template <typename OutputIt>
    Result insert_addrinfo(OutputIt iter,
                           const Hostname& node,
                           const Service& service,
                           const addrinfo* hints,
                           bool verbose)
    {
        const AiResult ai_result(get_addrinfo(node, service, hints, verbose));
        addrinfo* list = ai_result.first;

        if (list == NULL) {
            return ai_result.second;
        }

        for (const addrinfo* elem = list; elem != NULL; elem = elem->ai_next) {
            if (verbose) {
                std::cerr << "Fetched addrinfo "
                          << elem
                          << ':'
                          << std::endl
                          << *elem
                          << std::endl;
            }

            *iter++ = *elem;
        }

        ::freeaddrinfo(list);
        return ai_result.second;
    }
}

#endif
