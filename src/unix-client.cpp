#include "network/network.h"    // Buffer
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

int main(int argc, char *argv[])
{
    sockaddr_un addr {};
    long error {0};
    bool shutdown {false};
    int sock {0};

    // Create local socket.
    sock = ::socket(AF_UNIX, SOCK_SEQPACKET, 0);

    if (sock == -1) {
        std::perror("socket");
        std::exit(EXIT_FAILURE);
    }

    // For portability clear the whole structure, since some
    // implementations have additional (nonstandard) fields in the
    // structure.
    std::memset(&addr, 0, sizeof addr);

    // Connect socket to socket address.
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, SOCKET_NAME, sizeof addr.sun_path - 1);

    error = ::connect(sock, reinterpret_cast<const sockaddr*>(&addr),
                      sizeof addr);

    if (error == -1) {
        std::cerr << "Service is unavailable"
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Send arguments.

    for (int i = 1; i < argc; ++i) {
        error = ::write(sock, argv[i], std::strlen(argv[i]) + 1);

        if (error == -1) {
            std::perror("write");
            break;
        }

        if (std::strcmp(argv[1], "DOWN") == 0) {
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
