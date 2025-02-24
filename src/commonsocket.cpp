// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/commonsocket.h"               // CommonSocket
#include "network/accept.h"                     // accept()
#include "network/acceptresult.h"               // AcceptResult
#include "network/buffer.h"                     // Buffer
#include "network/bytestring.h"                 // ByteString
#include "network/close.h"                      // close()
#include "network/get-name.h"                   // get_name()
#include "network/getnameparams.h"              // GetNameParams
#include "network/listen.h"                     // listen()
#include "network/open-handle.h"                // open()
#include "network/openhandleparams.h"           // OpenHandleParams
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/read.h"                       // read()
#include "network/socketdata.h"                 // SocketData
#include "network/write.h"                      // write()

#include <cstddef>      // std::size_t
#include <iostream>     // std::cerr, std::endl

Network::CommonSocket::CommonSocket(const SocketData& t_data)
    : Socket(t_data)
{
}

Network::CommonSocket::~CommonSocket() noexcept
{
    if (const auto result {Network::close(handle(),
                                          is_verbose())}) {
        std::cerr << result.string()
                  << std::endl;
    }
}

auto Network::CommonSocket::accept() const -> AcceptResult
{
    return Network::accept(*this);
}

auto Network::CommonSocket::listen(int t_backlog) const -> OsErrorResult
{
    return Network::listen(handle(), t_backlog,
                           is_verbose());
}

auto Network::CommonSocket::name(bool t_is_peer) const -> ByteString
{
    const auto index {static_cast<std::size_t>(t_is_peer)};
    ByteString& value {m_names.at(index)};

    if (value.empty()) {
        const GetNameParams args
        {
            .m_handle = handle(), .m_is_verbose = is_verbose()
        };
        value = Network::get_name(args, t_is_peer);
    }

    return value;
}

auto Network::CommonSocket::open(const ByteString& t_addr,
                                 bool t_is_bind) -> OsErrorResult
{
    const OpenHandleParams args
    {
        .m_handle = handle(), .m_addr = t_addr, .m_is_verbose = is_verbose()
    };

    if (const auto result {Network::open(args, t_is_bind)}) {
        return result;
    }

    return {};
}

auto Network::CommonSocket::read(char* t_data,
                                 std::size_t t_size) const -> ssize_t
{
    return Network::read(handle(), t_data, t_size,
                         is_verbose());
}

auto Network::CommonSocket::read(std::size_t t_size) const -> ReadResult
{
    Buffer<char> buffer {t_size};
    const auto result {read(buffer.data(), buffer.size())};
    return {std::string {buffer}, result};
}

auto Network::CommonSocket::write(const char* t_data,
                                  std::size_t t_size) const -> ssize_t
{
    return Network::write(handle(),
                          t_data,
                          t_size,
                          is_verbose());
}

auto Network::CommonSocket::write(std::string_view t_sv) const -> ssize_t
{
    return write(t_sv.data(), t_sv.size());
}
