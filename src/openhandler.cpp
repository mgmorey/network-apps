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

#include "network/openhandler.hpp"      // OpenHandler
#include "network/symbol.hpp"           // Symbol

Network::OpenHandler::OpenHandler(Function t_function,
                                  String t_string,
                                  Symbol t_symbol) :
        m_function(t_function),
        m_string(t_string),
        m_symbol(t_symbol)
{
}

extern auto Network::OpenHandler::function() const noexcept -> Function
{
    return m_function;
}

extern auto Network::OpenHandler::string() const noexcept -> String
{
    return m_string;
}

extern auto Network::OpenHandler::symbol() const noexcept -> Symbol
{
    return m_symbol;
}
