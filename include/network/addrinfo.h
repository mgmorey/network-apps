#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

#include "network/endpoint.h"       // Endpoint
#include "network/hostname.h"       // get_hostname()
#include "network/hints.h"          // Hints
#include "network/result.h"         // Result
#include "network/types.h"          // Hostname, Service

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
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
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::input_iterator_tag;
            using pointer = addrinfo*;
            using reference = addrinfo&;
            using value_type = addrinfo;

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
            pointer m_pointer {nullptr};
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
            addrinfo* m_pointer {nullptr};
            Result m_result;
        };

        extern bool operator==(const InputIterator& left,
                               const InputIterator& right);
        extern bool operator!=(const InputIterator& left,
                               const InputIterator& right);

        template<typename OutputIt>
        Result insert(const Hostname& node,
                      const Service& serv,
                      const Hints* hints,
                      bool verbose,
                      OutputIt out)
        {
            const auto lambda = [&](const addrinfo& in) {
                if (verbose) {
                    std::cerr << "Fetched addrinfo:"
                              << std::endl
                              << in
                              << std::endl;
                }

                *out++ = in;
            };
            const auto list {List(node, serv, hints, verbose)};
            std::for_each(list.begin(), list.end(), lambda);
            return list.result();
        }
    }
}

#endif
