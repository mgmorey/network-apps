#include "network-address.h"    // Address
#include "network-close.h"      // close()
#include "network-connect.h"    // ConnectDetails, ConnectResult,
                                // Endpoint, Hostname, Service,
                                // connect()
#include "network-context.h"    // Context
#include "network-fd.h"         // sock_fd_type
#include "network-peername.h"   // AddressResult, get_peername()
#include "network-socket.h"     // Socket
#include "stream-address.h"     // operator<<()
#include "stream-container.h"   // operator<<()

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

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl

static void test_peer(Network::sock_fd_type sock_fd);

static void test_connect(const Network::Endpoint& endpoint,
                         const Network::Socket& hints)
{
    const Network::ConnectResult connect_result(connect(endpoint, hints, true));
    const Network::ConnectDetails connect_details(connect_result.second);
    const Network::sock_fd_type sock_fd = connect_result.first;

    if (sock_fd == Network::sock_fd_null) {
        std::cerr << connect_details;
        return;
    }

    const Network::Hostname cname = connect_details.front().string();
    const Network::Hostname hostname(endpoint.first);
    const Network::Service service(endpoint.second);
    std::cout << "Socket "
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
    std::cout << "Socket "
              << sock_fd
              << " closed"
              << std::endl;
}

static void test_peer(Network::sock_fd_type sock_fd)
{
    const Network::AddressResult
        address_result(Network::get_peername(sock_fd, true));
    const Network::Result result(address_result.second);

    if (result.nonzero()) {
        std::cerr << "No address: "
                  << result
                  << std::endl;
    }
    else {
        const Network::Address address(address_result.first);
        std::cout << "Socket "
                  << sock_fd
                  << " connected to "
                  << address
                  << std::endl;
    }
}

int main(int argc, char* argv[])
{
    static const Network::Socket
        hints(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, AI_CANONNAME);

    const Network::Context context;
    const Network::Hostname host(argc > 1 ? argv[1] : "example.com");
    const Network::Service service(argc > 2 ? argv[2] : "http");
    test_connect(Network::Endpoint(host, service), hints);
    static_cast<void>(context);
    return EXIT_SUCCESS;
}
