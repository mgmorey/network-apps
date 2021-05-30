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

    Addrinfos ai(get_addrinfo(host, service, &hints));
    int fd = -1;

    for (Addrinfos::const_iterator it = ai.begin();
         it != ai.end();
         ++it) {
        Socket socket(it->first);
        fd = socket.connect();

        if (fd >= 0) {
            break;
        }
    }

    if (fd >= 0) {
        std::cout << "Socket "
                  << fd
                  << " connected"
                  << std::endl;
        close(fd);
        std::cout << "Socket "
                  << fd
                  << " closed"
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

    if (error) {
        printf("WSAStartup() returned %d\n", error);
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
