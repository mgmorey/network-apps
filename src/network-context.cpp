#include "network-context.h"    // GlobalContext

#ifdef _WIN32
#include <winsock2.h>   // WSACleanup(), WSAStartup()
#endif

Network::Context::Context()
{
    if (!count++) {
#ifdef WIN32
        WSADATA wsaData;
        const int code = WSAStartup(MAKEWORD(2, 0), &wsaData);

        if (code != 0) {
            std::cerr << "WSAStartup() returned "
                      << code
                      << std::endl;
        }
#endif
    }
}

Network::Context::~Context()
{
    if (!--count) {
#ifdef WIN32
        WSACleanup();
#endif
    }
}
