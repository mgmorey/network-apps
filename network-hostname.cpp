#include "network-hostname.h"   // get_hostname()
#include "network-string.h"     // trim_zeros()

#ifdef _WIN32
#include <winsock2.h>   // gethostname()
#include <ws2tcpip.h>   // NI_MAXHOST
#else
#include <netdb.h>      // NI_MAXHOST
#include <unistd.h>     // gethostname()
#endif

#include <iostream>     // std::cerr, std::endl

Network::Hostname Network::get_hostname()
{
    Hostname hostname(NI_MAXHOST, '\0');
    int error = ::gethostname(&hostname[0], hostname.size() - 1);

    if (error != 0) {
        std::cerr << "gethostname(...) returned "
                  << error
                  << std::endl;
    }

    return resize(hostname);
}
