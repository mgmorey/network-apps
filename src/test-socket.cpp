#include "network/network.h"        // Address, FdPair, Hints,
                                    // Overload, Socket, close(),
                                    // get_peername(), get_sockaddr(),
                                    // get_socketpair(),
                                    // get_sockname()

#include <sys/socket.h> // AF_UNIX, SOCK_STREAM
#include <sys/un.h>     // sockaddr_un
#include <unistd.h>     // getopt(), optarg, opterr, optind, optopt

#include <cassert>      // assert()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string
#include <variant>      // std::visit()
#include <vector>       // std::vector

namespace TestSocket
{
    static constexpr auto PATH_12 {"/tmp/6789012"};
    static constexpr auto PATH_14 {"/tmp/678901234"};
    static constexpr auto PATH_16 {"/tmp/67890123456"};
    static constexpr auto PATH_20 {"/tmp/678901234567890"};

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
            args.emplace_back(argv[index]);
        }

        return args;
    }

    static void test_path(const std::string& path)
    {
        Network::Address address {Network::get_sockaddr(path)};
        assert(is_valid(address, verbose));
        std::cout << "Unix domain address: "
                  << address
                  << std::endl;
    }

    static void test_socketpair(const Network::Hints& hints)
    {
        const Network::Socket sock {hints};
        const auto socketpair_result {get_socketpair(sock, verbose)};
        std::visit(Network::Overload {
                [&](const Network::FdPair& fd) {
                    std::cout << "Socket "
                              << fd.first
                              << " connected to socket "
                              << fd.second
                              << std::endl;

                    Network::close(fd.first);
                    Network::close(fd.second);
                    std::cout << "Sockets "
                              << fd.first
                              << " and "
                              << fd.second
                              << " closed"
                              << std::endl;
                },
                [&](const Network::Result& result) {
                    std::cerr << result
                              << std::endl;
                }
            }, socketpair_result);
    }
}

int main(int argc, char* argv[])
{
    TestSocket::parse_arguments(argc, argv);
    const Network::Socket hints(AF_UNIX, SOCK_STREAM);
    TestSocket::test_socketpair(hints);
    TestSocket::test_path(TestSocket::PATH_12);
    TestSocket::test_path(TestSocket::PATH_14);
    TestSocket::test_path(TestSocket::PATH_16);
    TestSocket::test_path(TestSocket::PATH_20);
}
