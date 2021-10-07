#ifndef NETWORK_SOCKLEN_H
#define NETWORK_SOCKLEN_H

#ifndef _WIN32
#include <sys/socket.h> // socklen_t
#endif

namespace Network
{
#ifdef _WIN32
        using socklen_type = int;
#else
        using socklen_type = socklen_t;
#endif
}

#endif
