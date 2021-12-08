#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

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
#include <iterator>     // std::input_iterator_tag
#include <ostream>      // std::ostream

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
            using pointer = const addrinfo*;
            using reference = const addrinfo&;
            using value_type = addrinfo;

            // cppcheck-suppress noExplicitConstructor
            InputIterator(pointer t_pointer);  // NOLINT
            reference operator*() const;
            pointer operator->() const;
            InputIterator& operator++();

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
            static InputIterator end();

            List() = delete;
            List(const List&) = delete;
            List(const List&&) = delete;
            List(const Hostname& t_node,
                 const Service& t_serv,
                 const Hints* t_hints,
                 bool t_verbose);
            ~List();
            List& operator=(const List&) = delete;
            List& operator=(const List&&) = delete;
            [[nodiscard]] InputIterator begin() const;
            [[nodiscard]] Result result() const;

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
            const auto list {List(node, serv, hints, verbose)};
            std::for_each(list.begin(), List::end(),
                          [&](const addrinfo& in) {
                              if (verbose) {
                                  std::cerr << "Fetched addrinfo:"
                                            << std::endl
                                            << in
                                            << std::endl;
                              }

                              *out++ = in;
                          });
            return list.result();
        }
    }
}

#endif
