#include "network-hostname.h"   // get_hostname()

#ifdef _WIN32
#include <winsock2.h>   // WSACleanup(), WSAStartup()
#endif

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string

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
    Network::HostnameResult hostname_result;
    int result = EXIT_FAILURE;

    if (wsa_set_up()) {
        goto clean_up;
    }

    hostname_result = Network::get_hostname();

    if (!hostname_result.second.empty()) {
        std::cerr << hostname_result.second
                  << std::endl;
        result = EXIT_FAILURE;
    }
    else {
        std::cout << "Hostname: "
                  << hostname_result.first
                  << std::endl;
        result = EXIT_SUCCESS;
    }

clean_up:
    wsa_tear_down();
    return result;
}
