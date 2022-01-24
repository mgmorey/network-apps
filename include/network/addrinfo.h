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

#include "network/errorresult.h"        // ErrorResult
#include "network/optionalhints.h"      // OptionalHints
#include "network/optionalhostname.h"   // OptionalHostname
#include "network/optionalservice.h"    // OptionalService

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

#include <cstddef>      // std::ptrdiff_t
#include <iterator>     // std::input_iterator_tag
#include <ostream>      // std::ostream

namespace Network
{
    class AddrInfo
    {
    public:
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
            pointer m_list {nullptr};
        };

        static auto end() -> InputIterator;

        AddrInfo() = delete;
        AddrInfo(const AddrInfo&) = delete;
        AddrInfo(const AddrInfo&&) = delete;
        AddrInfo(const OptionalHostname& t_hostname,
             const OptionalService& t_service,
             const OptionalHints& t_hints,
             bool t_verbose);
        ~AddrInfo();
        auto operator=(const AddrInfo&) -> AddrInfo& = delete;
        auto operator=(const AddrInfo&&) -> AddrInfo& = delete;
        [[nodiscard]] auto begin() const -> InputIterator;
        [[nodiscard]] auto result() const -> ErrorResult;

    protected:
        static auto to_c_string(const OptionalString& str) -> const char*;

    private:
        addrinfo* m_list {nullptr};
        ErrorResult m_result;
    };

    extern auto operator==(const AddrInfo::InputIterator& left,
                           const AddrInfo::InputIterator& right) -> bool;
    extern auto operator!=(const AddrInfo::InputIterator& left,
                           const AddrInfo::InputIterator& right) ->  bool;
    extern auto operator<<(std::ostream& os,
                           const addrinfo& ai) -> std::ostream&;
}

#endif
