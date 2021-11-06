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

    Network::Address get_peer(const Network::Fd& t_fd)
    {
        const auto peername_result {Network::get_peername(t_fd, verbose)};
        const auto sock_addr {peername_result.first};
        const auto result {peername_result.second};

        if (result.result()) {
            std::cerr << "No peer information available: "
                      << result
                      << std::endl;
        }

        return sock_addr;
    }

    Network::Address get_sock(const Network::Fd& t_fd)
    {
        const auto sockname_result {Network::get_sockname(t_fd, verbose)};
        const auto sock_addr {sockname_result.first};
        const auto result {sockname_result.second};

        if (result.result()) {
            std::cerr << "No socket information available: "
                      << result
                      << std::endl;
        }

        return sock_addr;
    }

    static void test_peer(const Network::Fd& fd)
    {
        const auto peer {get_peer(fd)};
        const auto sock {get_sock(fd)};

        if (!peer.empty() && !sock.empty()) {
            std::cout << "Socket "
                      << fd
                      << " connected "
                      << sock
                      << " to "
                      << peer
                      << std::endl;
        }
    }

    static void test_socket(const std::string& path)
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
        const auto pair_result {sock.socketpair(verbose)};
        const auto fd {pair_result.first};
        const auto result {pair_result.second};

        if (result.result() != 0) {
            std::cerr << result
                      << std::endl;
        }
        else {
            std::cout << "Socket "
                      << fd.first
                      << " connected to socket "
                      << fd.second
                      << std::endl;
            test_peer(fd.first);
            test_peer(fd.second);
            Network::close(fd.first);
            Network::close(fd.second);
            std::cout << "Sockets "
                      << fd.first
                      << " and "
                      << fd.second
                      << " closed"
                      << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    TestSocket::parse_arguments(argc, argv);
    const Network::Socket hints(AF_UNIX, SOCK_STREAM);
    TestSocket::test_socketpair(hints);
    TestSocket::test_socket(TestSocket::PATH_12);
    TestSocket::test_socket(TestSocket::PATH_14);
    TestSocket::test_socket(TestSocket::PATH_16);
    TestSocket::test_socket(TestSocket::PATH_20);
}
