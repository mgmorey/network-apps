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
               << error
               << " ("
               << ::gai_strerror(result)
               << ')'
               << std::endl;

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
    Container get_addrinfo(std::string& error,
                           const Hostname& node,
                           const Service& service,
                           const struct addrinfo* hints)
    {
        Container result;
        copy_addrinfo(result, error, node, service, hints);
        result.unique();
        return result;
    }
}

#endif
