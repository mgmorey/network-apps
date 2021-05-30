#include "network.h"

#ifdef _WIN32
#include <winsock2.h>		// WSACleanup(), WSAStartup(), closesocket()
#endif

#include <cassert>		// assert()
#include <cstdlib>		// EXIT_FAILURE, EXIT_SUCCESS
#include <string>		// std::string
#include <iostream>		// std::cout, std::endl

static void print_addresses(const Addresses& addresses,
                            const std::string& family)
{
    if (addresses.empty()) {
        return;
    }

    std::cout << std::endl;
    std::cout << family
              << " addresses:"
              << std::endl
              << std::endl;

    for (Addresses::const_iterator it = addresses.begin();
         it != addresses.end();
         ++it)
    {
        std::string addr(it->to_string());
        std::string host(it->to_hostname());
        std::cout << '\t'
                  << addr;

        if (host != addr) {
            std::cout << " ("
                      << host
                      << ") ";
        }

        std::cout << std::endl;
    }
}

static void test_connect(const std::string& host, const std::string& service)
{
    struct addrinfo hints = {
        AI_CANONNAME,	// ai_flags
        AF_UNSPEC,	// ai_family
        SOCK_STREAM,	// ai_socktype
        IPPROTO_TCP,	// ai_protocol
        0,		// ai_adddrlen
        NULL,		// ai_canonname
        NULL,		// ai_addr
        NULL		// ai_next
    };

    Sockets sockets(get_addrinfo(host, service, &hints));
    std::string cname;
    int fd = -1;

    for (Sockets::const_iterator it = sockets.begin();
         it != sockets.end();
         ++it) {
        fd = it->connect();

        if (fd >= 0) {
            cname = it->get_cname();
            break;
        }
    }

    if (fd >= 0) {
        std::cout << "Connected to "
                  << cname
                  << " on socket "
                  << fd
                  << std::endl;
        Socket::close(fd);
        std::cout << "Closed socket "
                  << fd
                  << std::endl;
    }
}

static void test_host(const std::string& host)
{
    Addresses all(get_addresses(host));
    Addresses ipv4(get_addresses(host, AF_INET));
    Addresses ipv6(get_addresses(host, AF_INET6));
    std::cout << "Hostname: " << host << std::endl;
    print_addresses(all, "All");
    print_addresses(ipv4, "IPv4");
    print_addresses(ipv6, "IPv6");
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
    std::string hostname("example.com");
    std::string service("http");
    int result = EXIT_FAILURE;

    if (wsa_set_up()) {
        goto clean_up;
    }

    test_host(get_hostname());
    std::cout << std::endl;
    test_host(hostname);
    std::cout << std::endl;
    test_connect(hostname, service);
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
