#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

#include "network-types.h"

#ifdef _WIN32
#include <ws2tcpip.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo(),
                                // socklen_t
#else
#include <netdb.h>		// NI_MAXHOST, struct addrinfo,
                                // freeaddrinfo, getaddrinfo(),
                                // getnameinfo()
#endif

#include <cassert>		// assert()
#include <iostream>		// std::cerr, std::endl

template <class Container>
void copy_addrinfo(Container& dest,
                   const Hostname& node,
                   const Service& service,
                   const struct addrinfo* hints)
{
    struct addrinfo* list = NULL;
    assert(!(node.empty() && service.empty()));
    const char* c_node = node.empty() ? NULL : node.c_str();
    const char* c_service = service.empty() ? NULL : service.c_str();
    int error = getaddrinfo(c_node, c_service, hints, &list);

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
                  << gai_strerror(error)
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

        freeaddrinfo(list);
    }
}

extern void copy_addrinfo(struct addrinfo& dest, const struct addrinfo& src);
extern void init_addrinfo(struct addrinfo& dest,
                          int protocol = 0,
                          int socktype = 0,
                          int family = 0,
                          int flags = 0);

#endif
