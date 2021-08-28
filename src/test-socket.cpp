#include "network-address.h"    // Address
#include "network-close.h"      // close()
#include "network-fd.h"         // sock_fd_type
#include "network-host.h"       // Host
#include "network-peername.h"   // AddressResult, get_peername()
#include "network-result.h"     // Result
#include "network-socket.h"     // FdPair, Socket, SocketpairResult
#include "stream-address.h"     // operator<<()

#include <sys/socket.h> // AF_UNIX, SOCK_STREAM

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl

namespace TestSocket
{
    static void test_peer(Network::sock_fd_type sock_fd)
    {
        const auto address_result(Network::get_peername(sock_fd, true));
        const auto result(address_result.second);

        if (result.nonzero()) {
            std::cerr << "No address: "
                      << result
                      << std::endl;
        }
        else {
            Network::Address address(address_result.first);
            std::cout << "Socket "
                      << sock_fd
                      << " connected to "
                      << address
                      << std::endl;
        }
    }
}

int main(void)
{
    const Network::Socket socket(AF_UNIX, SOCK_STREAM);
    const auto socketpair_result(socket.socketpair(true));
    const auto result(socketpair_result.second);

    if (result.nonzero()) {
        std::cerr << result
                  << std::endl;
    }
    else {
        const auto sock_fd(socketpair_result.first);
        std::cout << "Socket "
                  << sock_fd.first
                  << " connected to socket "
                  << sock_fd.second
                  << std::endl;
        TestSocket::test_peer(sock_fd.first);
        TestSocket::test_peer(sock_fd.second);
        Network::close(sock_fd.first);
        Network::close(sock_fd.second);
        std::cout << "Sockets "
                  << sock_fd.first
                  << " and "
                  << sock_fd.second
                  << " closed"
                  << std::endl;
    }
    return EXIT_SUCCESS;
}
