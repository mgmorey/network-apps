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

#include "connection.hpp"       // NetworkExamples::Unix
#include "network/charspan.hpp" // CharSpan
#include "network/error.hpp"    // Error
#include "network/socket.hpp"   // Socket

#include <cstddef> // std::size_t
#include <string>  // std::string

namespace
{
    constexpr std::size_t buffer_size {
        NetworkExamples::Unix::message_size_max + 1,
    };
} // namespace

auto NetworkExamples::Unix::read_message(const Network::Socket& socket)
    -> std::string
{
    if constexpr (uses_stream_socket) {
        std::string message;

        for (;;) {
            char character {};
            const auto count {socket.read(Network::CharSpan {&character, 1})};

            if (count == 0) {
                throw Network::Error {
                    "Peer closed before the end of a message"};
            }

            if (character == '\n') {
                return message;
            }

            if (message.size() == NetworkExamples::Unix::message_size_max) {
                throw Network::Error {"Message exceeds maximum size"};
            }

            message.push_back(character);
        }
    }

    std::string message(buffer_size, '\0');
    const auto count {
        socket.read(Network::CharSpan {message.data(), message.size()}),
    };

    if (count == 0) {
        throw Network::Error {"Peer closed before sending a message"};
    }

    message.resize(static_cast<std::size_t>(count));
    if (message.size() > NetworkExamples::Unix::message_size_max) {
        throw Network::Error {"Message exceeds maximum size"};
    }

    return message;
}
