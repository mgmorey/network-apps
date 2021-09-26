#include "network-address.h"    // Address, operator<<()
#include "network-close.h"      // close()
#include "network-connect.h"    // ConnectResult, ConnectResults
                                // Endpoint, Hostname, Service,
                                // connect()
#include "network-context.h"    // Context
#include "network-fd.h"         // SocketFd
#include "network-peername.h"   // AddressResult, get_peername()
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
#include <iostream>     // std::cerr, std::cout, std::endl

namespace TestConnect
{
    static bool verbose {false};

    class Test
    {
    public:
        Test(const Network::Endpoint& t_endpoint,
             std::ostream& t_os) :
            m_endpoint(t_endpoint),
            m_os(t_os)
        {
        }

        void operator()(const Network::SocketResult& t_socket_result)
        {
            test_socket(t_socket_result);
        }

        void test_socket(const Network::SocketResult& t_socket_result)
        {
            const auto socket_fd = t_socket_result.first;
            const auto result = t_socket_result.second;

            if (!socket_fd) {
                std::cerr << result
                          << std::endl;
            }
            else {
                test_socket(socket_fd, result);
            }
        }

        void test_socket(Network::SocketFd t_socket_fd,
                         const Network::Result& t_result)
        {
            const auto cname {t_result.string()};
            const auto hostname {m_endpoint.first};
            const auto service {m_endpoint.second};
            m_os << "Socket "
                 << t_socket_fd
                 << " connected to "
                 << service
                 << " on "
                 << (cname.empty() ?
                     hostname :
                     cname)
                 << std::endl;
            test_socket_peer(t_socket_fd);
            Network::close(t_socket_fd);
            m_os << "Socket "
                 << t_socket_fd
                 << " closed"
                 << std::endl;
        }

        void test_socket_peer(Network::SocketFd t_socket_fd)
        {
            const auto address_result {Network::get_peername(t_socket_fd, true)};
            const auto address {address_result.first};
            const auto result {address_result.second};

            if (result.result() != 0) {
                std::cerr << "No address: "
                          << result
                          << std::endl;
            }
            else {
                m_os << "Socket "
                     << t_socket_fd
                     << " connected to "
                     << address
                     << std::endl;
            }
        }

    private:
        Network::Endpoint m_endpoint;
        std::ostream& m_os;
    };

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

    static void test_connect(const Network::Endpoint& endpoint,
                             const Network::Hints& hints)
    {
        const auto results {connect(endpoint, &hints, verbose)};
        std::for_each(results.begin(), results.end(),
                      Test(endpoint, std::cout));
    }
}

int main(int argc, char* argv[])
{
    constexpr auto host_default = "example.com";
    constexpr auto service_default = "http";

    static const Network::Hints hints(AF_UNSPEC,
                                      SOCK_STREAM,
                                      IPPROTO_TCP,
                                      AI_CANONNAME);

    const Network::Context context;

    if (!TestConnect::parse_arguments(argc, argv)) {
        exit(EXIT_FAILURE);
    }

    argc -= optind;
    argv += optind;
    const auto host {argc > 1 ? argv[1] : host_default};
    const auto service {argc > 2 ? argv[2] : service_default};
    const auto endpoint {Network::Endpoint(host, service)};
    TestConnect::test_connect(endpoint, hints);
    static_cast<void>(context);
}
