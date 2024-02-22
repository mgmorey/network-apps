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

#include "network/startup.h"            // startup()
#include "network/context.h"            // Context
#include "network/optionalversion.h"    // OptionalVersion

#ifdef WIN32
#include "network/error.h"              // Error
#include "network/format-os-error.h"    // format_os_error()
#include "network/logicerror.h"         // LogicError
#include "network/runtimeerror.h"       // RuntimeError
#include "network/to-os-error.h"        // to_os_error()
#include "network/version-null.h"       // version_null
#include "network/version.h"            // Version
#include "network/windowsversion.h"     // WORD, WSADATA, WSAEFAULT,
                                        // WSAEPROCLIM,
                                        // WSASYSNOTREADY,
                                        // WSAVERNOTSUPPORTED,
                                        // ::WSAStartup(),
                                        // WindowsVersion
#endif

#ifdef WIN32
static constexpr Network::Version version_default {2, 2};
#endif

auto Network::startup(Context& context, const OptionalVersion& version) -> void
{
#ifdef WIN32
    WSADATA wsa_data {};
    const WindowsVersion required_version {version.value_or(version_default)};

    if (const auto error {::WSAStartup(WORD {required_version}, &wsa_data)}) {
        const auto os_error {to_os_error(error)};
        const auto message {format_os_error(os_error)};

        switch (error) {  // NOLINT
        case WSAEFAULT:
        case WSAVERNOTSUPPORTED:
            throw LogicError {message};
        case WSAEPROCLIM:
        case WSASYSNOTREADY:
            throw RuntimeError {message};
        default:
            throw Error {message};
        }
    }

    context.m_description = wsa_data.szDescription;		// NOLINT
    context.m_system_status = wsa_data.szSystemStatus;		// NOLINT
    context.m_version = WindowsVersion {wsa_data.wVersion};
#else
    context.m_description = "Berkeley Software Distribution Sockets";
    context.m_version = version;
#endif
    context.m_is_started = true;
}
