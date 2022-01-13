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

#ifndef NETWORK_ITERATORPAIR_H
#define NETWORK_ITERATORPAIR_H

namespace Network
{
    template <typename It>
    struct IteratorPair
    {
        IteratorPair(It t_begin, It t_end) :
            m_begin(t_begin),
            m_end(t_end)
        {
        }

        [[nodiscard]] auto begin() const -> It
        {
            return m_begin;
        }

        [[nodiscard]] auto end() const -> It
        {
            return m_end;
        }

    private:
        It m_begin;
        It m_end;
    };
}

#endif
