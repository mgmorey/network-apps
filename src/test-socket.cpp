#include "network-close.h"      // close()
#include "network-host.h"       // Host

#include "network-socket.h"     // Socket

#include <sys/socket.h> // AF_UNIX, SOCK_DGRAM, SOCK_STREAM

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl

static void test_socket()
{
    Network::Socket socket(AF_UNIX, SOCK_STREAM, 0);
    Network::SocketPairResult socketpair_result(socket.socketpair(true));
    Network::Result result(socketpair_result.second);

    if (result.nonzero()) {
        std::cerr << result.string()
                  << std::endl;
    }
    else {
        Network::FdPair fds(socketpair_result.first);
        std::cout << "Opened sockets "
                  << fds.first
                  << " and "
                  << fds.second
                  << std::endl;
        Network::Host host(socket);
        Network::Result first_result(host.connect(fds.first, true));
        Network::Result second_result(host.connect(fds.second, true));
        Network::close(fds.first);
        Network::close(fds.second);
        std::cout << "Closed sockets "
                  << fds.first
                  << " and "
                  << fds.second
                  << std::endl;
    }
}

int main(void)
{
    test_socket();
    return EXIT_SUCCESS;
}
