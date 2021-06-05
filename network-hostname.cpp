#include "network-hostname.h"
#include "network-string.h"

#ifdef _WIN32
#include <winsock2.h>   // gethostname()
#include <ws2tcpip.h>   // NI_MAXHOST
#else
#include <netdb.h>      // NI_MAXHOST
#include <unistd.h>     // gethostname()
#endif

#include <iostream>     // std::cerr, std::endl
#include <string>       // std::string

Network::Hostname Network::get_hostname()
{
    Hostname host(NI_MAXHOST, '\0');
    int error = ::gethostname(&host[0], NI_MAXHOST - 1);

    if (error != 0) {
        std::cerr << "gethostname(...) returned "
                  << error
                  << std::endl;
    }

    trim_zeros(host);
    return host;
}
