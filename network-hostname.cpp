#include "network-hostname.h"   // HostnameResult, get_hostname()
#include "network-buffer.h"     // Buffer

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
    Buffer hostname(NI_MAXHOST);
    int code = ::gethostname(&hostname[0], hostname.size() - 1);

    if (code != 0) {
        std::ostringstream os;
        os << "gethostname(...) returned "
           << code;
        error = os.str();
    }

    return HostnameResult(hostname.data(), Result(code, error));
}
