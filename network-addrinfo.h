#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, freeaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, freeaddrinfo()
#endif

#include <cstddef>      // std::size_t
#include <iostream>     // std::cerr, std::endl
#include <ostream>      // std::ostream
#include <string>       // std::string
#include <utility>      // std::pair
#include <vector>       // std::vector

namespace Network
{
    typedef std::pair<addrinfo*, Result> AddrinfoResult;

    class Family
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Family& family);

    public:
        Family(int family);

    private:
        int value;
    };

    class Flags
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Flags& flags);

    public:
        Flags(int flags);

    private:
        int value;
    };

    class Name
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Name& name);

    public:
        Name(const std::string& name);
        Name(const char* name);

    private:
        bool null;
        const std::string value;
    };

    class Protocol
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Protocol& protocol);

    public:
        Protocol(int protocol);

    private:
        int value;
    };

    class SockType
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const SockType& socktype);

    public:
        SockType(int socktype);

    private:
        int value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const addrinfo& ai);
    extern std::ostream& operator<<(std::ostream& os,
                                    const Family& family);
    extern std::ostream& operator<<(std::ostream& os,
                                    const Flags& flags);
    extern std::ostream& operator<<(std::ostream& os,
                                    const Name& name);
    extern std::ostream& operator<<(std::ostream& os,
                                    const Protocol& protocol);
    extern std::ostream& operator<<(std::ostream& os,
                                    const SockType& socktype);
    extern AddrinfoResult get_addrinfo(const Hostname& node,
                                       const Service& service,
                                       const addrinfo* hints,
                                       bool verbose);
    extern void print_statistics(const std::vector<std::size_t>& sizes);

    template <class Container>
    std::size_t copy_addrinfo(Container& dest,
                              const Hostname& node,
                              const Service& service,
                              const addrinfo* hints,
                              bool verbose)
    {
        AddrinfoResult result(get_addrinfo(node, service, hints, verbose));
        addrinfo* list = result.first;
        std::size_t size = 0;
        dest.second = result.second;

        if (list == NULL) {
            return size;
        }

        for (const addrinfo* elem = list;
             elem != NULL;
             elem = elem->ai_next, ++size) {
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
        return size;
    }

    template <class Container>
    Container get_addrinfo(const Hostname& node,
                           const Service& service,
                           const addrinfo* hints,
                           bool verbose)
    {
        Container cont;
        std::vector<std::size_t> sizes;
        std::size_t size = copy_addrinfo(cont,
                                         node,
                                         service,
                                         hints,
                                         verbose);

        if (verbose) {
            sizes.push_back(size);
        }

        cont.first.unique();

        if (verbose) {
            sizes.push_back(cont.first.size());
            print_statistics(sizes);
        }

        return cont;
    }
}

#endif
