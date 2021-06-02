#include "network-address.h"	// Address
#include "network-addresses.h"	// Addresses, get_addresses()
#include "network-connect.h"	// close_socket(), connect_socket()
#include "network-hostname.h"	// get_hostname()
#include "network-socket.h"	// Socket

#ifdef _WIN32
#include <winsock2.h>		// IPPROTO_TCP, WSACleanup(), WSAStartup()
#else
#include <netinet/in.h>		// IPPROTO_TCP
#endif

#include <algorithm>		// std::copy, std::set_union()
#include <cstdlib>		// EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>		// std::cout, std::endl
#include <iterator>		// std::inserter()
#include <set>			// std::set
#include <string>		// std::string

static void test_connect(std::string& host, const std::string& service)
{
    Socket hints(IPPROTO_TCP, SOCK_STREAM, AF_UNSPEC, AI_CANONNAME);
    ConnectResult result = connect_socket(host, service, hints);
    std::string canonical_name = result.second;
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
