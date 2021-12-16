#ifndef NETWORK_SOCK_LEN_TYPE_H
#define NETWORK_SOCK_LEN_TYPE_H

#ifndef _WIN32
#include <sys/socket.h>     // socklen_t
#endif

namespace Network
{
#ifdef _WIN32
        using sock_len_type = int;
#else
        using sock_len_type = socklen_t;
#endif
}

#endif
