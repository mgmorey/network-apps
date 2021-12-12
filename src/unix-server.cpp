#include "network/network.h"    // Buffer, bind(), get_sockaddr()
#include "unix-common.h"        // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET, ::accept(),
                                // ::bind(), ::connect(), ::listen(),
                                // ::socket()
#include <sys/un.h>             // AF_UNIX
#include <unistd.h>             // ::close(), ::read(), ::unlink(),
                                // ::write()

#include <cstdio>       // std::fprintf(), std::perror(),
                        // std::printf()
#include <cstdlib>      // std::atexit(), std::exit(), std::strtol()
#include <cstring>      // std::memset(), std::strcmp(),
                        // std::strcpy(), std::strncpy()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string
#include <variant>      // std::get(), std::holds_alternative()

static constexpr auto backlog_size {20};
static constexpr auto radix {10};

static Network::Fd sock;  // NOLINT

static auto clean_up() -> void
{
    // Close the socket.
    if (sock >= 0) {
        std::cerr << "Closing socket "
                  << sock
                  << std::endl;
        sock = Network::close(sock);
    }

    // Unlink the socket.
    std::cerr << "Removing file "
              << SOCKET_NAME
              << std::endl;
    ::unlink(SOCKET_NAME);
}

auto main() -> int
{
    Network::Buffer buffer(BUFFER_SIZE);
    bool shutdown {false};

    // Create local socket.
    sock = ::socket(AF_UNIX, SOCK_SEQPACKET, 0);

    if (sock == -1) {
        std::perror("socket");
        std::exit(EXIT_FAILURE);
    }

    // Bind socket to socket name.
    const auto sock_addr {Network::get_sockaddr(SOCKET_NAME)};
    const auto sock_result {Network::bind(sock, sock_addr)};
    auto error {sock_result.result()};

    if (error == -1) {
        std::perror("bind");
        std::exit(EXIT_FAILURE);
    }

    error = std::atexit(clean_up);

    if (error == -1) {
        std::perror("atexit");
        std::exit(EXIT_FAILURE);
    }

    // Prepare for accepting connections. The backlog size is set to
    // 20. So while one request is being processed other requests can
    // be waiting.
    error = ::listen(sock, backlog_size);

    if (error == -1) {
        std::perror("listen");
        std::exit(EXIT_FAILURE);
    }

    // This is the main loop for handling connections.
    for (;;) {
        int data_socket = 0;

        // Wait for incoming connection.
        data_socket = ::accept(sock, nullptr, nullptr);

        if (data_socket == -1) {
            std::perror("accept");
            std::exit(EXIT_FAILURE);
        }

        long sum {0};

        for (;;) {

            // Wait for next data packet.
            error = ::read(data_socket, buffer.data(), buffer.size());

            if (error == -1) {
                std::perror("read");
                std::exit(EXIT_FAILURE);
            }

            // Handle commands.

            if (std::strncmp(buffer.data(), "DOWN", buffer.size()) == 0) {
                shutdown = true;
                break;
            }

            if (std::strncmp(buffer.data(), "END", buffer.size()) == 0) {
                break;
            }

            // Add received summand.
            const auto term {std::strtol(buffer.data(), nullptr, radix)};
            sum += term;
        }

        if (!shutdown) {
            // Send sum.
            std::ostringstream oss;
            oss << sum
                << '\0';
            std::string str {oss.str()};
            error = ::write(data_socket, str.data(), str.size());

            if (error == -1) {
                std::perror("write");
                std::exit(EXIT_FAILURE);
            }
        }

        // Close socket.

        ::close(data_socket);

        // Quit on DOWN command.

        if (shutdown) {
            break;
        }
    }
}
