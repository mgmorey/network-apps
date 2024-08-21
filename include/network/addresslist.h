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

#ifndef NETWORK_ADDRESSLIST_H
#define NETWORK_ADDRESSLIST_H

#include "network/optionalhints.h"      // OptionalHints
#include "network/optionalhostname.h"   // OptionalHostname
#include "network/optionalservice.h"    // OptionalService
#include "network/oserrorresult.h"      // OsErrorResult

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

#include <cstddef>      // std::ptrdiff_t
#include <iterator>     // std::input_iterator_tag
#include <memory>       // std::unique_ptr
#include <optional>     // std::optional
#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class AddressList
    {
        using OptionalString = std::optional<std::string>;
        friend auto operator<<(std::ostream& os,
                               const addrinfo& ai) -> std::ostream&;

    public:
        struct InputIterator
        {
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::input_iterator_tag;
            using pointer = const addrinfo*;
            using reference = const addrinfo&;
            using value_type = addrinfo;

            // cppcheck-suppress noExplicitConstructor; NOLINTNEXTLINE
            InputIterator(pointer t_pointer) noexcept;
            [[nodiscard]] auto operator*() const noexcept -> reference;
            [[nodiscard]] auto operator->() const noexcept -> pointer;
            auto operator++() noexcept -> InputIterator&;
            [[nodiscard]] auto operator==(const InputIterator& rhs)
                const noexcept -> bool;

        private:
            pointer m_list {nullptr};
        };

        AddressList() = delete;
        AddressList(const AddressList&) = delete;
        AddressList(const AddressList&&) = delete;
        AddressList(const OptionalHostname& t_hostname,
                    const OptionalService& t_service,
                    const OptionalHints& t_hints,
                    bool t_verbose);
        ~AddressList() noexcept;
        auto operator=(const AddressList&) noexcept -> AddressList& = delete;
        auto operator=(const AddressList&&) noexcept -> AddressList& = delete;
        [[nodiscard]] auto begin() const noexcept -> InputIterator;
        [[nodiscard]] auto end() const noexcept -> InputIterator;
        [[nodiscard]] auto result() const noexcept -> const OsErrorResult&;

    protected:
        static auto format(const OptionalHints& t_hints) -> std::string;
        static auto to_ai_ptr(const OptionalHints& t_hints)
            noexcept -> std::unique_ptr<addrinfo>;
        static auto to_c_str(const OptionalString& t_str)
            noexcept -> const char*;

    private:
        addrinfo* m_list {nullptr};
        OsErrorResult m_result;
    };

    extern auto operator<<(std::ostream& os,
                           const AddressList& list) -> std::ostream&;
    extern auto operator<<(std::ostream& os,
                           const addrinfo& ai) -> std::ostream&;
}

#endif
