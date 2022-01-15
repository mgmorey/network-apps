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

#ifndef NETWORK_ERROR_H
#define NETWORK_ERROR_H

#include <exception>   // std::exception
#include <string>      // std::string

namespace Network
{
    class Error : public std::exception
    {
    public:
        explicit Error(const std::string& t_what) :
            m_what(t_what)
        {
        }

        virtual auto what() const throw() -> const char*
        {
            return m_what.c_str();
        }

    private:
        std::string m_what;
    };
}

#endif
