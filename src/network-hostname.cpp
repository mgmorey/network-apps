#include "network-hostname.h"   // Hostname, HostnameResult,
                                // get_hostname()
#include "network-buffer.h"     // Buffer
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()

#ifdef _WIN32
#include <winsock2.h>   // gethostname()
#include <ws2tcpip.h>   // NI_MAXHOST
#else
#include <netdb.h>      // NI_MAXHOST
#include <unistd.h>     // gethostname()
#endif

#include <sstream>      // std::ostringstream

Network::HostnameResult Network::get_hostname()
{
    std::string error;
    Buffer hostname {NI_MAXHOST};
    auto code {reset_last_error()};

    if (::gethostname(&hostname[0], hostname.size() - 1) != 0) {
        code = get_last_error();
        std::ostringstream oss;
        oss << "Call to gethostname(...) returned "
            << code
            << ": "
            << format_error(code);
        error = oss.str();
    }

    return HostnameResult(hostname, Result(code, error));
}

Network::Hostname Network::get_hostname(const Network::Hostname& hostname)
{
    return hostname.empty() ? get_hostname().first : hostname;
}

Network::Hostname Network::get_hostname(const Network::Endpoint& endpoint)
{
    return endpoint.first.empty() ? get_hostname().first : endpoint.first;
}
