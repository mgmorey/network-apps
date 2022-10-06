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

#include "network/startup.h"            // Context, OptionalVersion,
                                        // Version, startup()
#include "network/exceptions.h"         // Error, LogicError,
                                        // RunTimeError
#include "network/format-os-error.h"    // format_os_error()
#include "network/rangeerror.h"         // RangeError
#include "network/windowsversion.h"     // Version, WORD, WSADATA,
                                        // WSAEFAULT, WSAEPROCLIM,
                                        // WSASYSNOTREADY,
                                        // WSAVERNOTSUPPORTED,
                                        // WSAStartup(),
                                        // WindowsVersion

#include <climits>      // INT_MAX, INT_MIN
#include <sstream>      // std::ostringstream

#ifdef WIN32
static constexpr auto version_default {Network::Version {2, 2}};
#else
static constexpr auto version_default {Network::Version {0, 0}};
#endif

auto Network::startup(Context& context, const OptionalVersion& version) -> void
{
#ifdef WIN32
    WSADATA wsa_data {};
    const WindowsVersion required_version {version.value_or(version_default)};
    const auto error {::WSAStartup(WORD {required_version}, &wsa_data)};

    if (error < 0 || error > INT_MAX) {
        std::ostringstream oss;
        oss << "Value "
            << error
            << " is out of range ["
            << 0
            << ", "
            << INT_MAX
            << "] of os_error_type";
        throw RangeError(oss.str());
    }
    else if (error != 0) {
        const auto os_error {static_cast<os_error_type>(error)};
        const auto message {format_os_error(os_error)};

        switch (error) {  // NOLINT
        case WSAEFAULT:
        case WSAVERNOTSUPPORTED:
            throw LogicError {message};
            break;
        case WSAEPROCLIM:
        case WSASYSNOTREADY:
            throw RuntimeError {message};
            break;
        default:
            throw Error {message};
        }
    }

    context.description(static_cast<const char*>(wsa_data.szDescription))
        .system_status(static_cast<const char*>(wsa_data.szSystemStatus))
        .version(Version {WindowsVersion {wsa_data.wVersion}});
#else
    const auto required_version {version.value_or(version_default)};
    context.description("Berkeley Software Distribution Sockets")
        .version(required_version);
#endif
    context.is_started(true);
}
