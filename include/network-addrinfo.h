#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

#include "network-endpoint.h"   // Endpoint
#include "network-hostname.h"   // get_hostname()
#include "network-hints.h"      // Hints
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <algorithm>    // std::for_each()
#include <cstddef>      // std::ptrdiff_t
#include <iostream>     // std::cerr, std::endl
#include <iterator>     // std::back_inserter(),
                        // std::input_iterator_tag
#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    extern std::ostream& operator<<(std::ostream& os,
                                    const addrinfo& ai);

    namespace AddrInfo
    {
        struct InputIterator
        {
            typedef std::ptrdiff_t difference_type;
            typedef std::input_iterator_tag iterator_category;
            typedef addrinfo* pointer;
            typedef addrinfo& reference;
            typedef addrinfo value_type;

            // cppcheck-suppress noExplicitConstructor
            InputIterator(pointer t_pointer);
            reference operator*() const;
            pointer operator->() const;
            InputIterator& operator++();
            InputIterator operator++(int);

            friend bool operator==(const InputIterator& left,
                                   const InputIterator& right);
            friend bool operator!=(const InputIterator& left,
                                   const InputIterator& right);

        private:
            pointer m_pointer;
        };

        class List
        {
        public:
            List(const Hostname& node,
                 const Service& service,
                 const Hints* hints,
                 bool verbose);
            ~List();
            InputIterator begin() const;
            InputIterator end() const;
            Result result() const;

        private:
            addrinfo* m_pointer;
            Result m_result;
        };

        extern bool operator==(const InputIterator& left,
                               const InputIterator& right);
        extern bool operator!=(const InputIterator& left,
                               const InputIterator& right);

        template<typename OutputIt>
        Result insert(const Hostname& node,
                      const Service& service,
                      const Hints* hints,
                      bool verbose,
                      OutputIt out)
        {
            auto lambda = [&](const addrinfo& in) {
                if (verbose) {
                    std::cerr << "Fetched addrinfo:"
                              << std::endl
                              << in
                              << std::endl;
                }

                *out++ = in;
            };
            const auto list(List(node, service, hints, verbose));
            std::for_each(list.begin(), list.end(), lambda);
            return list.result();
        }

        template<typename Container>
        Container get(const Endpoint& endpoint,
                      const Hints* hints,
                      bool verbose)
        {
            Container result;
            const auto node(get_hostname(endpoint));
            const auto service(endpoint.second);
            result.second = insert(node, service, hints, verbose,
                                   std::back_inserter(result.first));
            return result;
        }
    }
}

#endif
