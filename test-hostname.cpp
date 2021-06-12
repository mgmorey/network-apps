#include "network-hostname.h"   // get_hostname()

#ifdef _WIN32
#include <ws2tcpip.h>   // NI_NUMERICHOST
#include <winsock2.h>   // WSACleanup(), WSAStartup()
#else
#include <netdb.h>      // NI_NUMERICHOST
#endif

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cout, std::endl
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
    int result = EXIT_FAILURE;

    if (wsa_set_up()) {
        goto clean_up;
    }

    std::cout << "Hostname: " << Network::get_hostname() << std::endl;
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
