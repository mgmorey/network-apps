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
    std::string message;
    Buffer hostname {NI_MAXHOST};
    auto error {reset_last_error()};
    auto code {::gethostname(&hostname[0], hostname.size() - 1)};

    if (code != 0) {
        error = get_last_error();
        std::ostringstream oss;
        oss << "Call to gethostname(...) returned "
            << code
            << ": "
            << format_error(error);
        message = oss.str();
    }

    assert(error == 0 ?
           message == "" :
           message != "");
    const Result result(error, message);
    return HostnameResult(hostname, result);
}

Network::Hostname Network::get_hostname(const Network::Hostname& hostname)
{
    return hostname.empty() ? get_hostname().first : hostname;
}

Network::Hostname Network::get_hostname(const Network::Endpoint& endpoint)
{
    return endpoint.first.empty() ? get_hostname().first : endpoint.first;
}
