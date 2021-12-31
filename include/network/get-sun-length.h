#ifndef NETWORK_GET_SUN_LENGTH_H
#define NETWORK_GET_SUN_LENGTH_H

#ifndef _WIN32
#include <sys/un.h>         // sockaddr_un
#endif

#include <cstddef>      // std::size_t

namespace Network
{
#ifndef _WIN32
#ifdef HAVE_SOCKADDR_SA_LEN
    extern auto get_sun_length(const sockaddr_un* sun,
                               std::size_t size) -> std::size_t;
#endif
    extern auto get_sun_path_length(const sockaddr_un* sun,
                                    std::size_t size) -> std::size_t;
#endif
}

#endif
