// Copyright (C) 2026  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "connection.hpp"     // NetworkExamples::Unix
#include "network/error.hpp"  // Error
#include "network/socket.hpp" // Socket

#include <cstddef>     // std::size_t
#include <string>      // std::string
#include <string_view> // std::string_view

auto NetworkExamples::Unix::write_message(const Network::Socket& socket,
                                          std::string_view message) -> void
{
    if (message.size() > NetworkExamples::Unix::message_size_max) {
        throw Network::Error {"Message exceeds maximum size"};
    }

    if constexpr (uses_stream_socket) {
        std::string frame {message};
        frame.push_back('\n');
        std::size_t offset {};

        while (offset < frame.size()) {
            const auto count {
                socket.write(std::string_view {frame}.substr(offset)),
            };

            if (count <= 0) {
                throw Network::Error {"Socket write made no progress"};
            }

            offset += static_cast<std::size_t>(count);
        }

        return;
    }

    const auto count {socket.write(message)};
    if (count < 0 || static_cast<std::size_t>(count) != message.size()) {
        throw Network::Error {"Incomplete sequenced-packet write"};
    }
}
