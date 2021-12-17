#include "network/network.h"    // Buffer, connect(), get_sockaddr()
#include "unix-common.h"        // BUFFER_SIZE, SOCKET_NAME

#include <sys/socket.h>         // SOCK_SEQPACKET, ::connect(),
                                // ::socket()
#include <sys/un.h>             // AF_UNIX
#include <unistd.h>             // ::close(), ::read(), ::write()

#include <cerrno>       // errno
#include <cstdio>       // std::fprintf(), std::perror(),
                        // std::printf()
#include <cstdlib>      // std::exit()
#include <cstring>      // std::memset(), std::strcmp(),
                        // std::strcpy(), std::strncpy()
#include <iostream>     // std::cerr, std::endl
#include <span>         // std::span()

auto main(int argc, char *argv[]) -> int
{
    bool shutdown {false};

    // Create local socket.
    Network::Fd sock {::socket(AF_UNIX, SOCK_SEQPACKET, 0)};

    if (sock == -1) {
        std::perror("socket");
        std::exit(EXIT_FAILURE);
    }

    // Connect socket to socket address.
    const auto sock_addr {Network::get_sockaddr(SOCKET_NAME)};
    const auto sock_result {Network::connect(sock, sock_addr)};
    auto error {sock_result.result()};

    if (error == -1) {
        std::cerr << "Service is unavailable"
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Send arguments.

    const auto args = std::span(argv, size_t(argc));

    for (int i = 1; i < argc; ++i) {
        error = ::write(sock, args[i], std::strlen(args[i]) + 1);

        if (error == -1) {
            std::perror("write");
            break;
        }

        if (std::strcmp(args[1], "DOWN") == 0) {
            shutdown = true;
            break;
        }
    }

    if (!shutdown) {
        Network::Buffer buffer(BUFFER_SIZE);

        // Request result.
        std::strncpy(buffer.data(), "END", buffer.size());
        error = ::write(sock, buffer.data(), std::strlen(buffer.data()) + 1);

        if (error == -1) {
            std::perror("write");
            std::exit(EXIT_FAILURE);
        }

        // Receive result.
        error = ::read(sock, buffer.data(), buffer.size());

        if (error == -1) {
            std::perror("read");
            std::exit(EXIT_FAILURE);
        }

        std::cerr << "Result: "
                  << buffer
                  << std::endl;
    }

    // Close socket.
    ::close(sock);
}
