#include "network-context.h"    // Context, Result
#include "network-error.h"      // format_error()

#ifdef _WIN32
#include <winsock2.h>   // WSACleanup(), WSAStartup()
#include <windows.h>    // HIBYTE(), LOBYTE()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::Context::Context(bool t_verbose) :
    m_verbose(t_verbose)
{
#ifdef _WIN32
    if (!m_count++) {
        const auto error {::WSAStartup(m_version, &m_data)};

        if (error != 0) {
            std::ostringstream oss;
            oss << "Call to WSAStartup() returned "
                << error
                << ": "
                << format_error(error);
            m_result = {error, oss.str()};
        }
        else if (m_verbose) {
            std::cerr << "Microsoft Windows Sockets Data:"
                      << std::endl
                      << "    Description: "
                      << m_data.szDescription
                      << std::endl
                      << "    System Status: "
                      << m_data.szSystemStatus
                      << std::endl
                      << "    System Version: "
                      << static_cast<int>(LOBYTE(m_data.wVersion))
                      << '.'
                      << static_cast<int>(HIBYTE(m_data.wVersion))
                      << std::endl;
        }
    }
#endif
}

Network::Context::~Context()
{
#ifdef _WIN32
    if (!--m_count) {
        ::WSACleanup();
    }
#endif
}

Network::Result Network::Context::result() const
{
    return m_result;
}

#ifdef _WIN32
std::size_t Network::Context::m_count;
WSADATA Network::Context::m_data;
#endif
