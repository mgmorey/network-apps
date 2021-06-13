#include "network-connect.h"    // close_socket(), connect_socket()
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

static void test_connect(const Network::Hostname& hostname,
                         const Network::Service& service)
{
    Network::Socket hints(IPPROTO_TCP, SOCK_STREAM, AF_UNSPEC, AI_CANONNAME);
    Network::ConnectResult result =
        Network::connect_socket(hostname, service, hints);
    int fd = result.first;

    if (fd == Network::Socket::SOCKET_BAD) {
        std::string error = result.second;
        std::cerr << error
                  << std::endl;
    }
    else {
        Network::Hostname canonical_name = result.second;
        std::cout << "Connected to "
                  << (!canonical_name.empty() ?
                      canonical_name :
                      hostname)
                  << " on socket "
                  << fd
                  << std::endl;
        Network::close_socket(fd);
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

int main(void)
{
    Network::Hostname hostname("example.com");
    Network::Service service("http");
    int result = EXIT_FAILURE;

    if (wsa_set_up()) {
        goto clean_up;
    }

    test_connect(hostname, service);
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
