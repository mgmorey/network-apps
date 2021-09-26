#include "network-context.h"    // Context
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()

#ifdef _WIN32
#include <winsock2.h>   // WSACleanup(), WSAStartup()
#endif

#include <iostream>     // std::cerr, std::endl

Network::Context::Context()
{
    if (!m_count++) {
#ifdef WIN32
        WSADATA wsaData;
        const auto code {::WSAStartup(MAKEWORD(2, 0), &wsaData)};

        if (code != 0) {
            std::cerr << "Call to WSAStartup() returned "
                      << code
                      << ": "
                      << format_error(code);
        }
#endif
    }
}

Network::Context::~Context()
{
    if (!--m_count) {
#ifdef WIN32
        ::WSACleanup();
#endif
    }
}

std::size_t Network::Context::m_count;
