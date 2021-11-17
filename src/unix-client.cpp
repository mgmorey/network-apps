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

int main(int argc, char *argv[])
{
    sockaddr_un addr {};
    int down_flag = 0;
    int ret = 0;
    int data_socket = 0;

    // Create local socket.
    data_socket = ::socket(AF_UNIX, SOCK_SEQPACKET, 0);

    if (data_socket == -1) {
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

    ret = ::connect(data_socket, (const sockaddr*)&addr, sizeof addr);

    if (ret == -1) {
        std::fprintf(stderr, "The server is down.\n");
        std::exit(EXIT_FAILURE);
    }

    // Send arguments.

    for (int i = 1; i < argc; ++i) {
        ret = ::write(data_socket, argv[i], strlen(argv[i]) + 1);

        if (ret == -1) {
            std::perror("write");
            break;
        }

        if (!std::strcmp(argv[1], "DOWN")) {
            down_flag = 1;
            break;
        }
    }

    if (!down_flag) {
        char buffer[BUFFER_SIZE];

        // Request result.
        std::strcpy(buffer, "END");
        ret = ::write(data_socket, buffer, strlen(buffer) + 1);

        if (ret == -1) {
            std::perror("write");
            std::exit(EXIT_FAILURE);
        }

        // Receive result.
        ret = ::read(data_socket, buffer, sizeof(buffer));

        if (ret == -1) {
            std::perror("read");
            std::exit(EXIT_FAILURE);
        }

        // Ensure buffer is 0-terminated.
        buffer[sizeof(buffer) - 1] = 0;
        std::printf("Result = %s\n", buffer);
    }

    // Close socket.
    ::close(data_socket);
}
