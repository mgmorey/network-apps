#include "network-hostname.h"   // HostnameResult, get_hostname()
#include "network-string.h"     // resize()

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
    Hostname hostname(NI_MAXHOST, '\0');
    int result = ::gethostname(&hostname[0], hostname.size() - 1);

    if (result != 0) {
        std::ostringstream os;
        os << "gethostname(...) returned "
           << result;
        error = os.str();
    }

    return HostnameResult(resize(hostname), error);
}
