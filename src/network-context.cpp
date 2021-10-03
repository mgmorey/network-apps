#include "network-context.h"    // Context, Result
#include "network-error.h"      // format_error()

#ifdef _WIN32
#include <winsock2.h>   // WSACleanup(), WSAStartup()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::Context::Context(bool t_verbose)
{
    if (!m_count++) {
#ifdef _WIN32
        const auto error {::WSAStartup(m_version, &m_data)};

        if (error != 0) {
            std::ostringstream oss;
            oss << "Call to WSAStartup() returned "
                << error
                << ": "
                << format_error(error);
            m_result = {error, oss.str()};
        }
        else if (t_verbose) {
            std::cerr << "Microsoft Windows Sockets Data:"
                      << std::endl
                      << "\tDescription: "
                      << m_data.szDescription
                      << std::endl
                      << "\tSystem Status: "
                      << m_data.szSystemStatus
                      << std::endl;
        }
#endif
    }
}

Network::Context::~Context()
{
    if (!--m_count) {
#ifdef _WIN32
        ::WSACleanup();
#endif
    }
}

Network::Result Network::Context::result()
{
    return m_result;
}

std::size_t Network::Context::m_count;
WSADATA Network::Context::m_data;
