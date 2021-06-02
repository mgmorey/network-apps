#include "network-address.h"	// Address
#include "network-addresses.h"	// Addresses, get_addresses()
#include "network-connect.h"	// close_socket(), connect_socket()
#include "network-hostname.h"	// get_hostname()
#include "network-socket.h"	// Socket

#ifdef _WIN32
#include <ws2tcpip.h>		// AI_CANONNAME
#include <winsock2.h>		// AF_UNSPEC, IPPROTO_TCP,
                                // SOCK_STREAM, WSACleanup(),
                                // WSAStartup()
#else
#include <netdb.h>		// AI_CANNONNAME
#include <netinet/in.h>		// IPPROTO_TCP
#include <sys/socket.h>		// AF_UNSPEC, SOCK_STREAM
#endif

#include <cstdlib>		// EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>		// std::cout, std::endl

static void test_connect(const Hostname& host, const Service& service)
{
    Socket hints(IPPROTO_TCP, SOCK_STREAM, AF_UNSPEC, AI_CANONNAME);
    ConnectResult result = connect_socket(host, service, hints);
    Hostname canonical_name = result.second;
    int fd = result.first;

    if (fd >= 0) {
        std::cout << "Connected to "
                  << (!canonical_name.empty() ?
                      canonical_name :
                      host)
                  << " on socket "
                  << fd
                  << std::endl;
        close_socket(fd);
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
    Hostname host("example.com");
    Service service("http");
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
