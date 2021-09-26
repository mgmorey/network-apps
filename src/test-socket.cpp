#include "network-address.h"    // Address, operator<<()
#include "network-close.h"      // close()
#include "network-fd.h"         // SocketFd
#include "network-hints.h"      // Hints
#include "network-peername.h"   // AddressResult, get_peername()
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket, SocketFdPair,
                                // SocketpairResult

#include <sys/socket.h> // AF_UNIX, SOCK_STREAM
#include <unistd.h>     // getopt(), optarg, opterr, optind, optopt

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string
#include <vector>       // std::vector

namespace TestSocket
{
    static bool verbose {false};

    static std::vector<std::string> parse_arguments(int argc, char** argv)
    {
        std::vector<std::string> args {argv[0]};
        int ch {};

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
                std::exit(EXIT_FAILURE);
            default:
                abort();
            }
        }

        for (auto index = optind; index < argc; ++index) {
            args.push_back(argv[index]);
        }

        return args;
    }

    static void test_peer(Network::SocketFd socket_fd)
    {
        const auto address_result {Network::get_peername(socket_fd, true)};
        const auto result {address_result.second};

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

    static void test_socket(const Network::Hints& hints)
    {
        const Network::Socket sock {hints};
        const auto pair_result {sock.socketpair(verbose)};
        const auto socket_fd {pair_result.first};
        const auto result {pair_result.second};

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
    TestSocket::parse_arguments(argc, argv);
    const Network::Socket hints(AF_UNIX, SOCK_STREAM);
    TestSocket::test_socket(hints);
}
