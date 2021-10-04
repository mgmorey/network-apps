#ifndef NETWORK_SOCKLEN_H
#define NETWORK_SOCKLEN_H

#ifndef _WIN32
#include <sys/socket.h> // socklen_t
#endif

namespace Network
{
#ifdef _WIN32
        typedef int socklen_type;
#else
        typedef socklen_t socklen_type;
#endif
}

#endif
