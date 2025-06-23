// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/namehandler.hpp"      // NameHandler
#include "network/symbol.hpp"           // Symbol

#include <string_view>  // std::string_view

Network::NameHandler::NameHandler(NameFunction t_function,
                                  std::string_view t_string,
                                  Symbol t_symbol) :
        m_function(t_function),
        m_string(t_string),
        m_symbol(t_symbol)
{
}

extern auto Network::NameHandler::function() const noexcept -> NameFunction
{
    return m_function;
}

extern auto Network::NameHandler::string() const noexcept -> std::string_view
{
    return m_string;
}

extern auto Network::NameHandler::symbol() const noexcept -> Symbol
{
    return m_symbol;
}
