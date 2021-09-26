#include "network-context.h"    // Context, Result
#include "network-error.h"      // format_error()

#ifdef _WIN32
#include <winsock2.h>   // WSACleanup(), WSAStartup()
#endif

#include <sstream>      // std::ostringstream

Network::Context::Context()
{
    if (!m_count++) {
#ifdef _WIN32
        const auto code {::WSAStartup(MAKEWORD(2, 0), &m_data)};

        if (code != 0) {
            std::ostringstream oss;
            oss << "Call to WSAStartup() returned "
                << code
                << ": "
                << format_error(code);
            m_result = Result(code, oss.str());
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
