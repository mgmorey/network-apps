#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service
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
    typedef std::pair<addrinfo*, Result> AddrInfoResult;

    extern AddrInfoResult get_addrinfo(const Hostname& node,
                                       const Service& service,
                                       const addrinfo* hints,
                                       bool verbose);

    template<typename OutputIt>
    void insert_addrinfo(const addrinfo* in,
                         bool verbose,
                         OutputIt out)
    {
        for (; in != nullptr; in = in->ai_next) {
            if (verbose) {
                std::cerr << "Fetched addrinfo "
                          << in
                          << ':'
                          << std::endl
                          << *in
                          << std::endl;
            }

            *out++ = *in;
        }
    }

    template<typename OutputIt>
    Result insert_addrinfo(const Hostname& node,
                           const Service& service,
                           const addrinfo* hints,
                           bool verbose,
                           OutputIt out)
    {
        const AddrInfoResult
            result(get_addrinfo(node, service, hints, verbose));

        if (result.first != nullptr) {
            insert_addrinfo(result.first, verbose, out);
            ::freeaddrinfo(result.first);
        }

        return result.second;
    }
}

#endif
