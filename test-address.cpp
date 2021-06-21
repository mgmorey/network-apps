#include "network-address.h"    // Address, Hostname
#include "network-addresses.h"  // Addresses, get_addresses()
#include "network-endpoint.h"   // Endpoint
#include "network-hostname.h"   // get_hostname()
#include "network-socket.h"     // Socket

#ifdef _WIN32
#include <winsock2.h>   // IPPROTO_TCP, SOCK_STREAM, WSACleanup(),
                        // WSAStartup()
#else
#include <netinet/in.h> // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP
#include <sys/socket.h> // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // SOCK_DGRAM, SOCK_STREAM
#endif

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string

static Network::AddressesResult get_addresses(const Network::Hostname& host,
                                              const struct addrinfo& hints,
                                              bool verbose)
{
    return Network::get_addresses(host, "", hints, verbose);
}

static int get_family(const Network::Socket& socket)
{
    const struct addrinfo& ai = static_cast<const struct addrinfo&>(socket);
    return ai.ai_family;
}

static std::string get_family_description(int family)
{
    std::string result;

    switch (family) {
    case AF_INET:
        result = "IPv4";
        break;
    case AF_INET6:
        result = "IPv6";
        break;
    }

    return result;
}

static void print_addresses(const Network::Addresses& addresses, int family)
{
    if (addresses.empty()) {
        return;
    }

    std::string description(get_family_description(family));
    std::cout << (description.empty() ? "All" : description)
              << " addresses:"
              << std::endl;

    for (Network::Addresses::const_iterator it = addresses.begin();
         it != addresses.end();
         ++it)
    {
        Network::Hostname address(to_string(*it));

        if (address.empty()) {
            continue;
        }

        std::cout << '\t'
                  << address;

        Network::Hostname hostname(to_hostname(*it));

        if (hostname != address) {
            std::cout << " ("
                      << hostname
                      << ") ";
        }

        std::cout << std::endl;
    }
}

static void test_host(const Network::Hostname& host,
                      const Network::Socket& hints,
                      bool verbose = true)
{
    Network::AddressesResult addrs_result(get_addresses(host, hints, verbose));
    Network::Addresses addrs(addrs_result.first);
    Network::Result result(addrs_result.second);
    int family = get_family(hints);

    if (result.nonzero()) {
        std::string description(get_family_description(family));

        if (description.empty()) {
            std::cerr << "No";
        }
        else {
            std::cerr << "No "
                      << description;
        }

        std::cerr << " addresses: "
                  << result.string()
                  << std::endl;
    }
    else if (!addrs.empty()) {
        print_addresses(addrs, family);
    }
}

static void test_host(const Network::Hostname& host)
{
    if (host.empty()) {
        Network::Socket hints;
        test_host(host, hints, true);
    }
    else {
        std::cout << "Host: " << host << std::endl;
        Network::Socket hints4(IPPROTO_TCP, SOCK_STREAM, AF_INET);
        Network::Socket hints6(IPPROTO_TCP, SOCK_STREAM, AF_INET6);
        test_host(host, hints4, true);
        test_host(host, hints6, true);
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
    int result = EXIT_FAILURE;

    if (wsa_set_up()) {
        goto clean_up;
    }

    if (argc <= 1) {
        test_host("");
    }

    test_host(host);
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
