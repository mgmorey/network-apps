#include "network-address.h"    // Address, operator<<()
#include "network-close.h"      // close()
#include "network-fd.h"         // Fd
#include "network-hints.h"      // Hints
#include "network-peername.h"   // get_peername()
#include "network-result.h"     // Result
#include "network-sockaddr.h"   // get_sockaddr()
#include "network-socket.h"     // Socket, FdPair,
                                // SocketpairResult
#include "network-sockname.h"   // get_sockname()

#include <sys/socket.h> // AF_UNIX, SOCK_STREAM
#include <sys/un.h>     // sockaddr_un
#include <unistd.h>     // getopt(), optarg, opterr, optind, optopt

#include <cassert>      // assert()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string
#include <variant>      // std::get(), std::holds_alternative()
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
            args.push_back(argv[index]);
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

        if (std::holds_alternative<Network::Result>(socketpair_result)) {
            const auto result {std::get<Network::Result>(socketpair_result)};
            std::cerr << result
                      << std::endl;
        }
        else if (std::holds_alternative<Network::FdPair>(socketpair_result)) {
            const auto fd {std::get<Network::FdPair>(socketpair_result)};
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
        }
        else {
            abort();
        }
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
