#include "network-address.h"    // Address
#include "network-close.h"      // close()
#include "network-connect.h"    // ConnectResult, ConnectResults
                                // Endpoint, Hostname, Service,
                                // connect()
#include "network-context.h"    // Context
#include "network-fd.h"         // sock_fd_type
#include "network-peername.h"   // AddressResult, get_peername()
#include "network-socket.h"     // Socket
#include "stream-address.h"     // operator<<()
#include "stream-container.h"   // print()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP,
                        // SOCK_DGRAM, SOCK_STREAM
#include <ws2tcpip.h>   // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
#else
#include <netdb.h>      // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
#include <netinet/in.h> // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP
#include <sys/socket.h> // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // SOCK_DGRAM, SOCK_STREAM
#endif

#include <algorithm>    // std::for_each()
#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl

namespace TestConnect
{
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
            const auto sock_fd = t_socket_result.first;

            if (sock_fd == Network::sock_fd_null) {
                std::cerr << t_socket_result.second
                          << std::endl;
            }
            else {
                const auto cname(t_socket_result.second.string());
                const auto hostname(m_endpoint.first);
                const auto service(m_endpoint.second);
                m_os << "Socket "
                     << sock_fd
                     << " connected to "
                     << service
                     << " on "
                     << (cname.empty() ?
                         hostname :
                         cname)
                     << std::endl;
                test_peer(sock_fd);
                Network::close(sock_fd);
                m_os << "Socket "
                     << sock_fd
                     << " closed"
                     << std::endl;
            }
        }

        void test_peer(Network::sock_fd_type sock_fd)
        {
            const auto address_result(Network::get_peername(sock_fd, true));
            const auto result(address_result.second);

            if (result.nonzero()) {
                std::cerr << "No address: "
                          << result
                          << std::endl;
            }
            else {
                const auto address(address_result.first);
                m_os << "Socket "
                     << sock_fd
                     << " connected to "
                     << address
                     << std::endl;
            }
        }

    private:
        Network::Endpoint m_endpoint;
        std::ostream& m_os;
    };

    static void test_connect(const Network::Endpoint& endpoint,
                             const Network::Socket& hints)
    {
        const auto results(connect(endpoint, hints, true));
        std::for_each(results.begin(), results.end(),
                      Test(std::cout, endpoint));
    }
}

int main(int argc, char* argv[])
{
    static const Network::Socket hints(AF_UNSPEC,
                                       SOCK_STREAM,
                                       IPPROTO_TCP,
                                       AI_CANONNAME);

    const Network::Context context;
    const auto host(argc > 1 ? argv[1] : "example.com");
    const auto service(argc > 2 ? argv[2] : "http");
    const auto endpoint(Network::Endpoint(host, service));
    TestConnect::test_connect(endpoint, hints);
    static_cast<void>(context);
    return EXIT_SUCCESS;
}
