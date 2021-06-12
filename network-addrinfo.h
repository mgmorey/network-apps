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
#include <iostream>     // std::cerr, std::endl

namespace Network
{
    template <class Container>
    void copy_addrinfo(Container& dest,
                       const Hostname& node,
                       const Service& service,
                       const struct addrinfo* hints)
    {
        struct addrinfo* list = NULL;
        assert(!node.empty() || !service.empty());
        int error = ::getaddrinfo(node.empty() ? NULL : node.c_str(),
                                  service.empty() ? NULL : service.c_str(),
                                  hints,
                                  &list);

        if (error != 0) {
            std::cerr << "getaddrinfo(";

            if (!node.empty()) {
                std::cerr << '"'
                          << node
                          << '"';
            }
            else {
                std::cerr << "NULL";
            }

            std::cerr << ", ";

            if (!service.empty()) {
                std::cerr << '"'
                          << service
                          << "\", ";
            }

            std::cerr << "...) returned "
                      << error
                      << " ("
                      << ::gai_strerror(error)
                      << ')'
                      << std::endl;
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
        Container result;
        copy_addrinfo(result, node, service, hints);
        result.unique();
        return result;
    }
}

#endif
