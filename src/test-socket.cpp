#include "network/network.h"        // Address, FdPair, Hints,
                                    // Overload, Socket, close(),
                                    // get_sockaddr(),
                                    // get_socketpair()

#include <sys/socket.h> // AF_UNIX, SOCK_STREAM
#include <sys/un.h>     // sockaddr_un
#include <unistd.h>     // getopt(), optarg, opterr, optind, optopt

#include <cassert>      // assert()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span()
#include <string>       // std::string
#include <variant>      // std::visit()
#include <vector>       // std::vector

namespace TestSocket
{
    static constexpr auto PATH_12 {"/tmp/6789012"};
    static constexpr auto PATH_14 {"/tmp/678901234"};
    static constexpr auto PATH_16 {"/tmp/67890123456"};
    static constexpr auto PATH_20 {"/tmp/678901234567890"};

    static bool verbose {false};  // NOLINT

    static auto parse_arguments(int argc, char** argv) ->
        std::vector<std::string>
    {
        std::vector<std::string> result {*argv};
        int ch {};

        while ((ch = ::getopt(argc, argv, "v")) != -1) {
            switch (ch) {
            case 'v':
                verbose = true;
                break;
            case '?':
                std::cerr << "Usage: "
                          << *argv
                          << " [-v]"
                          << std::endl;
                std::exit(EXIT_FAILURE);
            default:
                abort();
            }
        }

        const auto args = std::span(argv, size_t(argc));

        for (auto index = optind; index < argc; ++index) {
            result.emplace_back(args[index]);
        }

        return result;
    }

    static auto test_path(const std::string& path) -> void
    {
        Network::Address address {Network::get_sockaddr(path)};
        assert(is_valid(address, verbose));  // NOLINT
        std::cout << "Unix domain address: "
                  << address
                  << std::endl;
    }

    static auto test_socketpair(const Network::Hints& hints) -> void
    {
        const Network::Socket sock {hints};
        const auto socketpair_result {get_socketpair(sock, verbose)};
        std::visit(Network::Overload {
                [&](const Network::FdPair& fds) {
                    std::cout << "Socket "
                              << fds[0]
                              << " connected to socket "
                              << fds[1]
                              << std::endl;

                    Network::close(fds[0]);
                    Network::close(fds[1]);
                    std::cout << "Sockets "
                              << fds[0]
                              << " and "
                              << fds[1]
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

auto main(int argc, char* argv[]) -> int
{
    try {
        TestSocket::parse_arguments(argc, argv);
        const Network::Socket hints(AF_UNIX, SOCK_STREAM);
        TestSocket::test_socketpair(hints);
        TestSocket::test_path(TestSocket::PATH_12);
        TestSocket::test_path(TestSocket::PATH_14);
        TestSocket::test_path(TestSocket::PATH_16);
        TestSocket::test_path(TestSocket::PATH_20);
    }
    catch (std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
