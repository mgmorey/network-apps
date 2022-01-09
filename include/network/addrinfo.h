// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef NETWORK_ADDRINFO_H
#define NETWORK_ADDRINFO_H

#include "network/hints.h"              // Hints
#include "network/hostname.h"           // Hostname
#include "network/result.h"             // Result
#include "network/service.h"            // Service

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

#include <cstddef>      // std::ptrdiff_t
#include <iostream>     // std::cerr, std::endl
#include <iterator>     // std::input_iterator_tag
#include <ostream>      // std::ostream

namespace Network
{
    extern auto operator<<(std::ostream& os,
                           const addrinfo& ai) -> std::ostream&;

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
            auto operator*() const -> reference;
            auto operator->() const -> pointer;
            auto operator++() -> InputIterator&;

            friend auto operator==(const InputIterator& left,
                                   const InputIterator& right) -> bool;
            friend auto operator!=(const InputIterator& left,
                                   const InputIterator& right) -> bool;

        private:
            pointer m_pointer {nullptr};
        };

        class List
        {
        public:
            static auto end() -> InputIterator;

            List() = delete;
            List(const List&) = delete;
            List(const List&&) = delete;
            List(const Hostname& t_node,
                 const Service& t_serv,
                 const Hints* t_hints,
                 bool t_verbose);
            ~List();
            auto operator=(const List&) -> List& = delete;
            auto operator=(const List&&) -> List& = delete;
            [[nodiscard]] auto begin() const -> InputIterator;
            [[nodiscard]] auto result() const -> Result;

        private:
            addrinfo* m_pointer {nullptr};
            Result m_result;
        };

        extern auto operator==(const InputIterator& left,
                               const InputIterator& right) -> bool;
        extern auto operator!=(const InputIterator& left,
                               const InputIterator& right) ->  bool;

        template<typename OutputIt>
        auto insert(const Hostname& node,
                    const Service& service,
                    const Hints* hints,
                    bool verbose,
                    OutputIt out) -> Result
        {
            const auto list {List(node, service, hints, verbose)};

            for (const auto& in : list) {
                if (verbose) {
                    std::cerr << "Fetched addrinfo:"
                              << std::endl
                              << in
                              << std::endl;
                }

                *out++ = in;
            }

            return list.result();
        }
    }
}

#endif
