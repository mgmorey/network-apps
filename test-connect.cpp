#include "network-connect.h"    // Result, Hostname, Service,
                                // close_socket(), connect_socket()
#include "network-socket.h"     // Socket

#ifdef _WIN32
#include <ws2tcpip.h>   // AI_CANONNAME
#include <winsock2.h>   // AF_UNSPEC, IPPROTO_TCP, SOCK_STREAM,
                        // WSACleanup(), WSAStartup()
#else
#include <netdb.h>      // AI_CANNONNAME
#include <netinet/in.h> // IPPROTO_TCP
#include <sys/socket.h> // AF_UNSPEC, SOCK_STREAM
#endif

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl

static void test_connect(const Network::Hostname& host,
                         const Network::Service& service)
{
    Network::Socket hints(IPPROTO_TCP, SOCK_STREAM, AF_UNSPEC, AI_CANONNAME);
    Network::ConnectResult result(Network::connect(host, service, hints, true));
    Network::ConnectDetails details(result.second);
    int fd = result.first;

    if (fd == Network::Socket::socket_bad) {
        for (Network::ConnectDetails::const_iterator it = details.begin();
             it != details.end();
             ++it) {
            std::cerr << *it
                      << std::endl;
        }
    }
    else {
        Network::Hostname name = details[0];
        std::cout << "Connected to "
                  << (!name.empty() ?
                      name :
                      host)
                  << " on socket "
                  << fd
                  << std::endl;
        Network::close(fd);
        std::cout << "Closed socket "
                  << fd
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

    test_connect(host, service);
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
