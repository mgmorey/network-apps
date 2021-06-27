#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network-buffer.h"     // Buffer
#include "network-result.h"     // Result
#include "network-sockaddr.h"   // SockAddr
#include "network-types.h"      // Hostname, Service

#include <ostream>      // std::ostream
#include <string>       // std::string
#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<Hostname, Service> Endpoint;

    class EndpointResult
    {
    public:
        EndpointResult(const SockAddr& address, int flags = 0, bool verbose = false);
        Endpoint endpoint() const;
        Hostname hostname() const;
        Service service() const;
        Result result() const;

    private:
        int code;
        std::string error;
        Buffer host_buffer;
        Buffer service_buffer;
    };

    extern EndpointResult to_endpoint(const SockAddr& address,
                                      bool numeric = false,
                                      bool verbose = false);
    extern Hostname to_hostname(const SockAddr& address,
                                bool verbose = false);
    extern Hostname to_string(const SockAddr& address,
                              bool verbose = false);
}

#endif
