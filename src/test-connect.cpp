#include "network-address.h"    // Address
#include "network-close.h"      // close()
#include "network-connect.h"    // ConnectDetails, ConnectResult,
                                // Endpoint, Hostname, Service,
                                // connect()
#include "network-fd.h"         // sock_fd_type
#include "network-peername.h"   // AddressResult, get_peername()
#include "network-socket.h"     // Socket

#ifdef _WIN32
#include <ws2tcpip.h>   // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
#include <winsock2.h>   // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP,
                        // SOCK_DGRAM, SOCK_STREAM, WSACleanup(),
                        // WSAStartup()
#else
#include <netdb.h>      // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
#include <netinet/in.h> // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP
#include <sys/socket.h> // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // SOCK_DGRAM, SOCK_STREAM
#endif

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl

static void test_peer(Network::sock_fd_type fd)
{
    Network::AddressResult address_result(Network::get_peername(fd, true));
    Network::Result result(address_result.second);

    if (result.nonzero()) {
        std::cerr << "No address: "
                  << result
                  << std::endl;
    }
    else {
        Network::Address address(address_result.first);
        std::cout << "Socket "
                  << fd
                  << " connected to "
                  << address
                  << std::endl;
    }
}

static void test_connect(const Network::Endpoint& endpoint)
{
    Network::Socket hints(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, AI_CANONNAME);
    Network::ConnectResult connect_result(connect(endpoint, hints, true));
    Network::ConnectDetails details(connect_result.second);
    Network::sock_fd_type fd = connect_result.first;

    if (fd == Network::sock_fd_null) {
        for (Network::ConnectDetails::const_iterator it = details.begin();
             it != details.end();
             ++it) {
            std::cerr << (*it)
                      << std::endl;
        }
    }
    else {
        Network::Hostname cname = details.front().string();
        Network::Hostname hostname(endpoint.first);
        Network::Service service(endpoint.second);
        std::cout << "Socket "
                  << fd
                  << " connected to "
                  << service
                  << " on "
                  << (cname.empty() ?
                      hostname :
                      cname)
                  << std::endl;
        test_peer(fd);
        Network::close(fd);
        std::cout << "Socket "
                  << fd
                  << " closed"
                  << std::endl;
    }
}

static int wsa_set_up(void)
{
    int error = 0;

#ifdef WIN32
    WSADATA wsaData;
    error = WSAStartup(MAKEWORD(2, 0), &wsaData);

    if (error != 0) {
        std::cerr << "WSAStartup() returned "
                  << error
                  << std::endl;
    }
#endif

    return error;
}

static void wsa_tear_down(void)
{
#ifdef WIN32
    WSACleanup();
#endif
}

int main(int argc, char* argv[])
{
    Network::Hostname host(argc > 1 ? argv[1] : "example.com");
    Network::Service service(argc > 2 ? argv[2] : "http");
    int result = EXIT_FAILURE;

    if (wsa_set_up()) {
        goto clean_up;
    }

    test_connect(Network::Endpoint(host, service));
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
