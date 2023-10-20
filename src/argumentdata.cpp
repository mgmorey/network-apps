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

#include "network/argument.h"           // Argument
#include "network/argumentdata.h"       // ArgumentData

#include <algorithm>    // std::for_each, std::transform()
#include <cstdlib>      // std::free(), std::size_t
#include <cstring>      // ::strdup()
#include <iterator>     // std::back_inserter()
#include <string>       // std::string
#include <vector>       // std::vector

Network::ArgumentData::ArgumentData(const std::vector<std::string>& data)
{
    std::transform(data.begin(), data.end(),
                   std::back_inserter(m_args),
                   [&](const std::string& arg) {
                       return ::strdup(arg.c_str());  // NOLINT
                   });
}

Network::ArgumentData::~ArgumentData()
{
    std::for_each(m_args.begin(), m_args.end(), std::free);
}

auto Network::ArgumentData::data() -> Argument*
{
    return m_args.data();
}

auto Network::ArgumentData::size() const -> std::size_t
{
    return m_args.size();
}
