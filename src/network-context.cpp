#include "network-context.h"    // GlobalContext

#ifdef _WIN32
#include <winsock2.h>   // WSACleanup(), WSAStartup()
#endif

#include <iostream>     // std::cerr, std::endl

Network::Context::Context()
{
    if (!m_count++) {
#ifdef WIN32
        WSADATA wsaData;
        const auto code {WSAStartup(MAKEWORD(2, 0), &wsaData)};

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
    if (!--m_count) {
#ifdef WIN32
        WSACleanup();
#endif
    }
}

std::size_t Network::Context::m_count;
