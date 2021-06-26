#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network-address.h"    // Address
#include "network-buffer.h"     // Buffer
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
        EndpointResult(const Address& address, int flags = 0, bool verbose = false);
        Endpoint endpoint() const;
        Hostname hostname() const;
        Service service() const;
        Result result() const;

    private:
        int code;
        std::string error;
        Buffer host;
        Buffer serv;
    };

    extern EndpointResult to_endpoint(const Address& address,
                                      bool numeric = false,
                                      bool verbose = false);
    extern Hostname to_hostname(const Address& address,
                                bool verbose = false);
    extern Hostname to_string(const Address& address,
                              bool verbose = false);
}

#endif
