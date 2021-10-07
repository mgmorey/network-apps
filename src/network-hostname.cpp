#include "network-hostname.h"   // Hostname, HostnameResult,
                                // get_hostname()
#include "network-buffer.h"     // Buffer
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
#include "network-result.h"     // Result

#ifdef _WIN32
#include <winsock2.h>   // gethostname()
#include <ws2tcpip.h>   // NI_MAXHOST
#else
#include <netdb.h>      // NI_MAXHOST
#include <unistd.h>     // gethostname()
#endif

#include <cassert>      // assert()
#include <sstream>      // std::ostringstream

Network::HostnameResult Network::get_hostname()
{
    Result result;
    Buffer host {NI_MAXHOST};
    auto error {reset_last_error()};

    if (::gethostname(&host[0], host.size() - 1)) {
        error = get_last_error();
        std::ostringstream oss;
        oss << "Call to gethostname(...) failed with error "
            << error
            << ": "
            << format_error(error);
        result = {error, oss.str()};
    }

    return HostnameResult(host, result);
}

Network::Hostname Network::get_hostname(const Network::Hostname& hostname)
{
    return hostname.empty() ? get_hostname().first : hostname;
}

Network::Hostname Network::get_hostname(const Network::Endpoint& endpoint)
{
    return endpoint.first.empty() ? get_hostname().first : endpoint.first;
}
