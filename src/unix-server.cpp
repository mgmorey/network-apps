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

static int connection_socket = -1;  // NOLINT

static void clean_up()
{
    // Close the socket.
    if (connection_socket >= 0) {
        std::cerr << "Closing socket"
                  << connection_socket
                  << '.'
                  << std::endl;
        ::close(connection_socket);
        connection_socket = -1;
    }

    // Unlink the socket.
    std::cerr << "Removing file"
              << SOCKET_NAME
              << '.'
              << std::endl;
    ::unlink(SOCKET_NAME);
}

int main()
{
    sockaddr_un name {};
    int down_flag = 0;
    long ret = 0;
    char buffer[BUFFER_SIZE];

    // Create local socket.
    connection_socket = ::socket(AF_UNIX, SOCK_SEQPACKET, 0);

    if (connection_socket == -1) {
        std::perror("socket");
        std::exit(EXIT_FAILURE);
    }

    // For portability clear the whole structure, since some
    // implementations have additional (nonstandard) fields in the
    // structure.
    std::memset(&name, 0, sizeof name);

    // Bind socket to socket name.
    name.sun_family = AF_UNIX;
    std::strncpy(name.sun_path, SOCKET_NAME, sizeof name.sun_path - 1);
    ret = ::bind(connection_socket,
                 reinterpret_cast<const sockaddr*>(&name),
                 sizeof name);

    if (ret == -1) {
        std::perror("bind");
        std::exit(EXIT_FAILURE);
    }

    ret = std::atexit(clean_up);

    if (ret == -1) {
        std::perror("atexit");
        std::exit(EXIT_FAILURE);
    }

    // Prepare for accepting connections. The backlog size is set to
    // 20. So while one request is being processed other requests can
    // be waiting.
    const auto backlog_size {20};
    ret = ::listen(connection_socket, backlog_size);

    if (ret == -1) {
        std::perror("listen");
        std::exit(EXIT_FAILURE);
    }

    // This is the main loop for handling connections.
    for (;;) {
        int data_socket = 0;

        // Wait for incoming connection.
        data_socket = ::accept(connection_socket, nullptr, nullptr);

        if (data_socket == -1) {
            std::perror("accept");
            std::exit(EXIT_FAILURE);
        }

        long result = 0;

        for (;;) {

            // Wait for next data packet.
            ret = ::read(data_socket, buffer, sizeof buffer);

            if (ret == -1) {
                std::perror("read");
                std::exit(EXIT_FAILURE);
            }

            // Ensure buffer is 0-terminated.
            buffer[sizeof buffer - 1] = 0;

            // Handle commands.

            if (std::strncmp(buffer, "DOWN", sizeof buffer) == 0) {
                down_flag = 1;
                break;
            }

            if (std::strncmp(buffer, "END", sizeof buffer) == 0) {
                break;
            }

            // Add received summand.
            const auto radix {10};
            result += std::strtol(buffer, nullptr, radix);
        }

        if (down_flag != 0) {
            // Send result.

            std::sprintf(buffer, "%ld", result);
            ret = ::write(data_socket, buffer, sizeof buffer);

            if (ret == -1) {
                std::perror("write");
                std::exit(EXIT_FAILURE);
            }
        }

        // Close socket.

        ::close(data_socket);

        // Quit on DOWN command.

        if (down_flag) {
            break;
        }
    }
}
