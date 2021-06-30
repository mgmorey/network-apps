#include "network-close.h"      // close()
#include "network-socket.h"     // Socket

#include <sys/socket.h> // AF_UNIX, SOCK_DGRAM, SOCK_STREAM

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl

static void test_socket()
{
    Network::Socket socket(AF_UNIX, SOCK_STREAM, 0);
    Network::SocketResult socket_result(socket.socketpair(true));
    Network::SocketPair socket_pair(socket_result.first);
    Network::Result result = socket_result.second;

    if (result.nonzero()) {
        std::cerr << result.string()
                  << std::endl;
    }
    else {
        std::cout << "Connected socket "
                  << socket_pair.first
                  << " to socket "
                  << socket_pair.second
                  << std::endl;
        Network::close(socket_pair.second);
        Network::close(socket_pair.first);
        std::cout << "Closed sockets "
                  << socket_pair.first
                  << " and "
                  << socket_pair.second
                  << std::endl;
    }
}

int main(void)
{
    test_socket();
    return EXIT_SUCCESS;
}
