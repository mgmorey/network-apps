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

#ifndef NETWORK_FORMAT_H
#define NETWORK_FORMAT_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Format
    {
        friend auto operator<<(std::ostream& os,
                               const Format& format) noexcept -> std::ostream&;

    public:
        explicit Format(const std::string& t_key) noexcept;
        explicit Format(std::string&& t_key) noexcept;
        explicit Format(std::string::size_type t_indent,
                        const std::string& t_key = {}) noexcept;
        Format(std::string::size_type t_indent,
               std::string&& t_key) noexcept;
        Format(const std::string& t_delimiter,
               std::string::size_type t_indent,
               const std::string& key = {});
        Format(const std::string& t_delimiter,
               std::string::size_type t_indent,
               std::string&& key);
        Format(std::string&& t_delimiter,
               std::string::size_type indent,
               const std::string& key = {});
        Format(std::string&& t_delimiter,
               std::string::size_type t_indent,
               std::string&& key);

    private:
        static constexpr const char* m_delimiter_default {","};
        static constexpr std::size_t m_indent_default {std::string::npos};

        std::string m_delimiter {m_delimiter_default};
        std::string::size_type m_indent {m_indent_default};
        std::string m_key;
    };

    extern auto operator<<(std::ostream& os,
                           const Format& format) noexcept -> std::ostream&;
}

#endif
