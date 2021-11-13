#include "network/hostname.h"   // Hostname, HostnameResult,
                                // get_hostname()
#include "network/buffer.h"     // Buffer
#include "network/error.h"      // format_error(), get_last_error(),
                                // reset_last_error()
#include "network/result.h"     // Result

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
    Buffer host_buffer {NI_MAXHOST};
    reset_last_error();

    if (::gethostname(host_buffer.data(), host_buffer.size() - 1)) {
        auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to gethostname(...) failed with error "
            << error
            << ": "
            << format_error(error);
        return Result(error, oss.str());
    }

    return OptionalString(host_buffer);
}

Network::HostnameResult Network::get_hostname(const Network::Hostname& host)
{
    return host.null() ? get_hostname() : host;
}
