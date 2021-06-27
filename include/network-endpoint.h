#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network-buffer.h"     // Buffer
#include "network-host.h"       // Host
#include "network-result.h"     // Result
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
        EndpointResult(const Host& host, int flags = 0, bool verbose = false);
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

    extern EndpointResult to_endpoint(const Host& host,
                                      bool numeric = false,
                                      bool verbose = false);
    extern Hostname to_hostname(const Host& host,
                                bool verbose = false);
    extern Hostname to_string(const Host& host,
                              bool verbose = false);
}

#endif
