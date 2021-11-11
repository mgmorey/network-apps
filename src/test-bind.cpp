#include "network-address.h"    // Address, operator<<()
#include "network-bind.h"       // Address, Endpoint, Fd, Hints,
                                // Socket, SocketResult, bind()
#include "network-close.h"      // close()
#include "network-context.h"    // Context
#include "network-fd.h"         // Fd
#include "network-hostname.h"   // get_hostname()
#include "network-peername.h"   // get_peername()
#include "network-sockname.h"   // get_sockname()
#include "network-socket.h"     // Socket

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
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string
#include <vector>       // std::vector

namespace TestBind
{
    static constexpr auto HOST {"localhost"};
    static constexpr auto SERVICE {"8080"};

    static bool verbose {false};

    class Test
    {
    public:
        Test(const Network::Endpoint& t_endp,
             std::ostream& t_os) :
            m_endpoint(t_endp),
            m_os(t_os)
        {
        }

        void operator()(const Network::SocketResult& t_socket_result)
        {
            test_socket(t_socket_result);
        }

        Network::SockAddrResult get_sockname(const Network::Fd& t_fd)
        {
            const auto sockname_result {Network::get_sockname(t_fd, verbose)};

            if (std::holds_alternative<Network::Result>(sockname_result)) {
                const auto result {std::get<Network::Result>(sockname_result)};
                std::cerr << "No host information available: "
                          << result
                          << std::endl;
            }

            return sockname_result;
        }

        void test_socket(const Network::SocketResult& t_socket_result)
        {
            const auto fd {t_socket_result.first};
            const auto result {t_socket_result.second};

            if (!fd) {
                std::cerr << result
                          << std::endl;
            }
            else {
                test_socket(fd, result);
            }
        }

        void test_socket(const Network::Fd& t_fd,
                         const Network::Result& t_result)
        {
            const auto cname {t_result.string()};
            const auto host {m_endpoint.first};
            const auto serv {m_endpoint.second};
            m_os << "Socket "
                 << t_fd
                 << " bound to "
                 << static_cast<std::string>(serv)
                 << " on "
                 << static_cast<std::string>(host)
                 << std::endl;
            test_socket(t_fd);
            Network::close(t_fd);
            m_os << "Socket "
                 << t_fd
                 << " closed"
                 << std::endl;
        }

        void test_socket(const Network::Fd& t_fd)
        {
            const auto sockname_result {get_sockname(t_fd)};

            if (std::holds_alternative<Network::SockAddr>(sockname_result)) {
                const auto self {std::get<Network::SockAddr>(sockname_result)};
                m_os << "Socket "
                     << t_fd
                     << " bound to "
                     << Network::Address(self)
                     << std::endl;
            }
        }

    private:
        Network::Endpoint m_endpoint;
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

    static void test_bind(const Network::Endpoint& endp,
                          const Network::Hints& hints)
    {
        const auto results {Network::bind(endp, &hints, verbose)};
        std::for_each(results.begin(), results.end(),
                      Test(endp, std::cout));
    }
}

int main(int argc, char* argv[])
{
    static const Network::Hints hints(AF_UNSPEC,
                                      SOCK_STREAM,
                                      IPPROTO_TCP,
                                      AI_CANONNAME);

    const auto args {TestBind::parse_arguments(argc, argv)};
    const Network::Context context(TestBind::verbose);

    if (context.result().result()) {
        std::cerr << context.result()
                  << std::endl;
    }
    else {
        const auto host {args.size() > 1 ? args[1] : TestBind::HOST};
        const auto serv {args.size() > 2 ? args[2] : TestBind::SERVICE};
        const auto endp {Network::Endpoint(host, serv)};
        TestBind::test_bind(endp, hints);
    }

    static_cast<void>(context);
}
