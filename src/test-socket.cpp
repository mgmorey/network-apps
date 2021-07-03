#include "network-address.h"    // Address
#include "network-close.h"      // close()
#include "network-fd.h"         // sock_fd_type
#include "network-host.h"       // Host
#include "network-peername.h"   // AddressResult, get_peername()
#include "network-socket.h"     // Socket
#include "stream-address.h"     // operator<<()

#include <sys/socket.h> // AF_UNIX, SOCK_DGRAM, SOCK_STREAM

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl

static void test_peer(Network::sock_fd_type fd)
{
    const Network::AddressResult addr_result(Network::get_peername(fd, true));
    const Network::Result result(addr_result.second);

    if (result.nonzero()) {
        std::cerr << "No address: "
                  << result
                  << std::endl;
    }
    else {
        Network::Address address(addr_result.first);
        std::cout << "Socket "
                  << fd
                  << " connected to "
                  << address
                  << std::endl;
    }
}

int main(void)
{
    const Network::Socket socket(AF_UNIX, SOCK_STREAM);
    const Network::SocketpairResult socketpair_result(socket.socketpair(true));
    const Network::Result result(socketpair_result.second);

    if (result.nonzero()) {
        std::cerr << result
                  << std::endl;
    }
    else {
        const Network::FdPair fds(socketpair_result.first);
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
    return EXIT_SUCCESS;
}
