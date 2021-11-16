#include "network/network.h"        // Address, Endpoint, FdResult,
                                    // Hints, Hostname, Overload,
                                    // SockAddr, SockAddrResult,
                                    // close(), connect()
                                    // get_peername(), get_sockaddr(),
                                    // get_sockname()

#ifdef _WIN32
#include <getopt.h>     // getopt(), optarg, opterr, optind, optopt
#include <winsock2.h>   // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP,
                        // SOCK_DGRAM, SOCK_STREAM
#include <ws2tcpip.h>   // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
#else
#include <netdb.h>      // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
#include <netinet/in.h> // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP
#include <sys/socket.h> // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // SOCK_DGRAM, SOCK_STREAM
#include <unistd.h>     // getopt(), optarg, opterr, optind, optopt
#endif

#include <algorithm>    // std::for_each()
#include <cassert>      // assert()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string
#include <variant>      // std::get(), std::holds_alternative,
                        // std::visit()
#include <vector>       // std::vector

namespace TestConnect
{
    static constexpr auto HOST {"example.com"};
    static constexpr auto SERVICE {"http"};

    static bool verbose {false};

    class Test
    {
    public:
        Test(const Network::Endpoint& t_endpoint,
             const Network::Hostname& t_hostname,
             std::ostream& t_os) :
            m_endpoint(t_endpoint),
            m_hostname(t_hostname),
            m_os(t_os)
        {
        }

        void operator()(const Network::FdResult& t_socket_result)
        {
            std::visit(Network::Overload {
                    [&](Network::Fd fd) {
                        test_socket(fd);
                    },
                    [&](const Network::Result& result) {
                        std::cerr << result
                                  << std::endl;
                    }
                }, t_socket_result);
        }

        Network::SockAddrResult get_peeraddr(const Network::Fd& t_fd)
        {
            const auto peername_result {Network::get_peername(t_fd, verbose)};
            std::visit(Network::Overload {
                    [&](const Network::SockAddr&) {
                    },
                    [&](const Network::Result& result) {
                        std::cerr << result
                                  << std::endl;
                    }
                }, peername_result);
            return peername_result;
        }

        Network::SockAddrResult get_sockaddr(const Network::Fd& t_fd)
        {
            const auto sockname_result {Network::get_sockname(t_fd, verbose)};
            std::visit(Network::Overload {
                    [&](const Network::SockAddr&) {
                    },
                    [&](const Network::Result& result) {
                        std::cerr << result
                                  << std::endl;
                    }
                }, sockname_result);
            return sockname_result;
        }

        void test_socket(const Network::Fd& t_fd)
        {
            const auto host {m_endpoint.first};
            const auto serv {m_endpoint.second};
            const auto peer_result {get_peeraddr(t_fd)};
            const auto sock_result {get_sockaddr(t_fd)};
            m_os << "Socket "
                 << t_fd
                 << " connected "
                 << m_hostname
                 << " to "
                 << serv
                 << " on "
                 << host
                 << std::endl;

            if (std::holds_alternative<Network::SockAddr>(peer_result) &&
                std::holds_alternative<Network::SockAddr>(sock_result)) {
                const auto peer {std::get<Network::SockAddr>(peer_result)};
                const auto self {std::get<Network::SockAddr>(sock_result)};
                m_os << "Socket "
                     << t_fd
                     << " connected "
                     << Network::Address(self)
                     << " to "
                     << Network::Address(peer)
                     << std::endl;
            }

            Network::close(t_fd);
            m_os << "Socket "
                 << t_fd
                 << " closed"
                 << std::endl;
        }

    private:
        Network::Endpoint m_endpoint;
        Network::Hostname m_hostname;
        std::ostream& m_os;
    };

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

    static void test_connect(const Network::Endpoint& endpoint,
                             const Network::Hints& hints)
    {
        const auto hostname_result {Network::get_hostname()};
        std::visit(Network::Overload {
                [&](const std::string& hostname) {
                    const auto socket_results {
                        Network::connect(endpoint, &hints, verbose)
                    };
                    assert(!socket_results.empty());
                    std::for_each(socket_results.begin(), socket_results.end(),
                                  Test(endpoint, hostname, std::cout));
                },
                [&](const Network::Result& result) {
                    std::cerr << "No hostname available: "
                              << result
                              << std::endl;
                }
            }, hostname_result);
    }
}

int main(int argc, char* argv[])
{
    static const Network::Hints hints(AF_UNSPEC,
                                      SOCK_STREAM,
                                      IPPROTO_TCP,
                                      AI_CANONNAME);

    const auto args {TestConnect::parse_arguments(argc, argv)};
    const Network::Context context(TestConnect::verbose);

    if (context.result()) {
        std::cerr << context.result()
                  << std::endl;
    }
    else {
        const auto host {args.size() > 1 ? args[1] : TestConnect::HOST};
        const auto serv {args.size() > 2 ? args[2] : TestConnect::SERVICE};
        const auto endp {Network::Endpoint(host, serv)};
        TestConnect::test_connect(endp, hints);
    }

    static_cast<void>(context);
}
