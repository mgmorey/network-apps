#include "network-address.h"    // Address, operator<<()
#include "network-close.h"      // close()
#include "network-fd.h"         // SocketFd
#include "network-host.h"       // Host
#include "network-peername.h"   // AddressResult, get_peername()
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket, SocketFdPair,
                                // SocketpairResult

#include <sys/socket.h> // AF_UNIX, SOCK_STREAM
#include <unistd.h>     // getopt(), optarg, opterr, optind, optopt

#include <iostream>     // std::cerr, std::cout, std::endl

namespace TestSocket
{
    static bool verbose = false;

    static bool parse_arguments(int& argc, char* argv[])
    {
        int ch;

        while ((ch = ::getopt(argc, argv, "v")) != -1) {
            switch (ch) {
            case 'v':
                verbose = true;
                break;
            case '?':
                std::cerr << "Usage: "
                          << argv[0]
                          << " [-v]"
                          << std::endl;
                return false;
            default:
                abort();
            }
        }

        return true;
    }

    static void test_peer(Network::SocketFd socket_fd)
    {
        const auto address_result(Network::get_peername(socket_fd, true));
        const auto result(address_result.second);

        if (result.result() != 0) {
            std::cerr << "No address: "
                      << result
                      << std::endl;
        }
        else {
            Network::Address address(address_result.first);
            std::cout << "Socket "
                      << socket_fd
                      << " connected to "
                      << address
                      << std::endl;
        }
    }

    static void test_socket(const Network::Socket& unix_socket)
    {
        const auto pair_result(unix_socket.socketpair(verbose));
        const auto socket_fd(pair_result.first);
        const auto result(pair_result.second);

        if (result.result() != 0) {
            std::cerr << result
                      << std::endl;
        }
        else {
            std::cout << "Socket "
                      << socket_fd.first
                      << " connected to socket "
                      << socket_fd.second
                      << std::endl;
            test_peer(socket_fd.first);
            test_peer(socket_fd.second);
            Network::close(socket_fd.first);
            Network::close(socket_fd.second);
            std::cout << "Sockets "
                      << socket_fd.first
                      << " and "
                      << socket_fd.second
                      << " closed"
                      << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    if (!TestSocket::parse_arguments(argc, argv)) {
        exit(EXIT_FAILURE);
    }

    const Network::Socket unix_socket(AF_UNIX, SOCK_STREAM);
    TestSocket::test_socket(unix_socket);
}
