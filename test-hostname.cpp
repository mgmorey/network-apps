#include "network-hostname.h"   // Hostname, HostnameResult, Result,
                                // get_hostname()

#ifdef _WIN32
#include <winsock2.h>   // WSACleanup(), WSAStartup()
#endif

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string

static void test_hostname()
{
    Network::HostnameResult hostname_result(Network::get_hostname());
    Network::Hostname hostname(hostname_result.first);
    Network::Result result(hostname_result.second);

    if (result.nonzero()) {
        std::cerr << "No hostname: "
                  << result.string()
                  << std::endl;
    }
    else {
        std::cout << "Hostname: "
                  << hostname
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
    int result = EXIT_FAILURE;

    if (wsa_set_up()) {
        goto clean_up;
    }

    test_hostname();
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
