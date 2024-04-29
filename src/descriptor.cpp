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

#include "network/descriptor.h"                 // Descriptor
#include "network/close.h"                      // close()
#include "network/descriptor-null.h"            // descriptor_null
#include "network/descriptor-type.h"            // descriptor_type
#ifndef WIN32
#include "network/remove.h"                     // remove()
#include "network/to-path-descriptor.h"         // to_path()
#endif

#ifndef WIN32
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::endl
#endif

Network::Descriptor::Descriptor(descriptor_type t_handle,
                                bool t_verbose) noexcept :
    m_handle(t_handle),
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

auto Network::Descriptor::bound(bool t_bound) noexcept -> void
{
#ifndef WIN32
    try {
        if (t_bound) {
            m_path = to_path(m_handle, m_verbose);
        }
        else if (m_path) {
            Network::remove(*m_path, m_verbose);
            m_path.reset();
        }
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
#else
    static_cast<void>(t_bound);
#endif
}

auto Network::Descriptor::close() noexcept -> Descriptor&
{
    if (m_handle == descriptor_null) {
        return *this;
    }

    if (const auto result {Network::close(m_handle, m_verbose)}) {
        std::cerr << result.string()
                  << std::endl;
    }
    else {
        m_handle = descriptor_null;
    }

    bound(false);
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
