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

#include "network/descriptor.h"                 // Descriptor,
                                                // descriptor_null,
                                                // descriptor_type
#include "network/close.h"                      // close()
#include "network/remove-socket.h"              // remove_socket()

#include <iostream>     // std::cerr, std::endl

Network::Descriptor::Descriptor(descriptor_type t_handle,
                                bool t_pending,
                                bool t_verbose) noexcept :
    m_handle(t_handle),
    m_pending(t_pending),
    m_verbose(t_verbose)
{
}

Network::Descriptor::~Descriptor() noexcept
{
    static_cast<void>(close());
}

auto Network::Descriptor::operator=(descriptor_type value) noexcept ->
    Descriptor&
{
    m_handle = value;
    return *this;
}

auto Network::Descriptor::close() noexcept -> Descriptor&
{
    if (m_handle == descriptor_null) {
        return *this;
    }

    if (m_pending) {
        try {
            remove_socket(m_handle, m_verbose);
        }
        catch (const std::exception& error) {
            std::cerr << error.what()
                      << std::endl;
        }
    }

    m_handle = Network::close(m_handle, m_verbose);
    return *this;
}

auto Network::Descriptor::handle() const noexcept -> descriptor_type
{
    return m_handle;
}

auto Network::Descriptor::verbose() const noexcept -> bool
{
    return m_verbose;
}
