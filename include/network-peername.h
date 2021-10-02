#ifndef NETWORK_PEERNAME_H
#define NETWORK_PEERNAME_H

#include "network-address.h"    // Address
#include "network-buffer.h"     // Buffer
#include "network-fd.h"         // Fd
#include "network-result.h"     // Result

#ifdef _WIN32
#include <winsock2.h>   // struct sockaddr, socklen_t
#else
#include <sys/socket.h> // struct sockaddr, socklen_t
#endif

#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<Address, Result> AddressResult;

    class AddressBuffer :
        public Buffer
    {
    public:
        AddressBuffer();
        sockaddr* addr();
        socklen_t addrlen() const;

    private:
        static std::size_t capacity();
    };

    extern AddressResult get_peername(Fd fd,
                                      bool verbose = false);
}

#endif
