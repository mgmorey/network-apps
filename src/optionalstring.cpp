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

#include "network/optionalstring.h" // OptionalString

Network::OptionalString::OptionalString(const std::nullopt_t& t_value) :
    m_value(t_value)
{
}

Network::OptionalString::OptionalString(const std::string& t_value) :
    m_value(t_value)
{
}

Network::OptionalString::OptionalString(const char* t_value)
{
    if (t_value != nullptr) {
        m_value = t_value;
    }
}

auto Network::OptionalString::operator=(const std::nullopt_t& t_value) ->
    Network::OptionalString&
{
    m_value = t_value;
    return *this;
}

auto Network::OptionalString::operator=(const std::string& t_value) ->
    Network::OptionalString&
{
    m_value = t_value;
    return *this;
}

auto Network::OptionalString::operator=(const char* t_value) ->
    Network::OptionalString&
{
    if (t_value == nullptr) {
        m_value.reset();
    }
    else {
        m_value = t_value;
    }

    return *this;
}

Network::OptionalString::operator std::string() const
{
    return m_value.value_or("");
}

Network::OptionalString::operator const char*() const
{
    return m_value.has_value() ? m_value.value().c_str() : nullptr;
}

auto Network::OptionalString::has_value() const -> bool
{
    return m_value.has_value();
}

auto Network::OptionalString::size() const -> std::string::size_type
{
    return m_value.has_value() ? m_value.value().size() : 0;
}

auto Network::OptionalString::value() const -> std::string
{
    return m_value.value_or("");
}

auto Network::OptionalString::value_or(const std::string& t_value) const ->
    std::string
{
    return m_value.value_or(t_value);
}
