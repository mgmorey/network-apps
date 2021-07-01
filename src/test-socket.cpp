#include "network-close.h"      // close()
#include "network-fd.h"         // fd_type
#include "network-host.h"       // Host
#include "network-peername.h"   // get_peername()
#include "network-socket.h"     // Socket

#include <sys/socket.h> // AF_UNIX, SOCK_DGRAM, SOCK_STREAM

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl

static void test_peer(Network::fd_type fd)
{
    Network::SockAddrResult sa_result(Network::get_peername(fd, true));
    Network::Result result(sa_result.second);

    if (result.nonzero()) {
        std::cerr << result.string()
                  << std::endl;
    }
    else {
        Network::SockAddr sockaddr(sa_result.first);
        std::cout << "Socket "
                  << fd
                  << " connected to "
                  << sockaddr
                  << std::endl;
    }
}

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
        std::cout << "Socket "
                  << fds.first
                  << " connected to socket "
                  << fds.second
                  << std::endl;
        test_peer(fds.first);
        test_peer(fds.second);
        Network::close(fds.first);
        Network::close(fds.second);
        std::cout << "Sockets "
                  << fds.first
                  << " and "
                  << fds.second
                  << " closed"
                  << std::endl;
    }
}

int main(void)
{
    test_socket();
    return EXIT_SUCCESS;
}
