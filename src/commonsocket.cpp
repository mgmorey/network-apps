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
#include "network/bytestring.h"                 // ByteString
#include "network/close.h"                      // close()
#include "network/family-null.h"                // family_null
#include "network/family-type.h"                // family_type
#include "network/familyerror.h"                // FamilyError
#include "network/get-name.h"                   // get_name()
#include "network/getnameparams.h"              // GetNameParams
#include "network/handle-null.h"                // handle_null
#include "network/handle-type.h"                // handle_type
#include "network/listen.h"                     // listen()
#include "network/logicerror.h"                 // LogicError
#include "network/open-handle.h"                // open()
#include "network/openhandleparams.h"           // OpenHandleParams
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/read.h"                       // read()
#include "network/shutdown.h"                   // shutdown()
#include "network/socketdata.h"                 // SocketData
#include "network/textbuffer.h"                 // TextBuffer
#include "network/write.h"                      // write()

#include <cstddef>      // std::size_t
#include <iostream>     // std::cerr, std::endl

Network::CommonSocket::CommonSocket(handle_type t_handle,
                                    family_type t_family,
                                    bool t_is_verbose,
                                    bool t_is_testing) :
    m_is_verbose(t_is_verbose),
    m_is_testing(t_is_testing)
{
    static_cast<void>(handle(t_handle));
    static_cast<void>(family(t_family));
}

Network::CommonSocket::CommonSocket(const SocketData& t_sd,
                                    handle_type t_handle) :
    CommonSocket(t_handle,
                 t_sd.family(),
                 t_sd.is_verbose(),
                 t_sd.is_testing())
{
}

Network::CommonSocket::CommonSocket(const SocketData& t_sd) :
    CommonSocket(t_sd.handle(),
                 t_sd.family(),
                 t_sd.is_verbose(),
                 t_sd.is_testing())
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

auto Network::CommonSocket::family() const noexcept -> family_type
{
    return m_family;
}

auto Network::CommonSocket::family(family_type t_family) -> CommonSocket&
{
    if (!is_testing() && t_family == family_null) {
        throw FamilyError(t_family);
    }

    m_family = t_family;
    return *this;
}

auto Network::CommonSocket::handle() const noexcept -> handle_type
{
    return m_handle;
}

auto Network::CommonSocket::handle(handle_type t_handle) -> CommonSocket&
{
    if (!is_testing() && t_handle == handle_null) {
        throw LogicError("Invalid socket descriptor value");
    }

    m_handle = t_handle;
    return *this;
}

auto Network::CommonSocket::is_verbose() const noexcept -> bool
{
    return m_is_verbose;
}

auto Network::CommonSocket::is_testing() const noexcept -> bool
{
    return m_is_testing;
}

auto Network::CommonSocket::accept() const -> AcceptResult

{
    const SocketData sd {m_handle, m_family, m_is_verbose, m_is_testing};
    return Network::accept(sd);
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

auto Network::CommonSocket::shutdown(int t_how) const -> OsErrorResult
{
    return Network::shutdown(handle(), t_how, is_verbose());
}

auto Network::CommonSocket::read(std::size_t t_size) const -> ReadResult
{
    TextBuffer buffer {t_size};
    const auto result {read(buffer.data(), buffer.size())};
    return {buffer, result};
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
