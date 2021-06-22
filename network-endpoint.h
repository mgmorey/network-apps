#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network-address.h"    // Address
#include "network-buffer.h"     // Buffer
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Endpoint
    {
    public:
        Endpoint(const Address& address, int flags = 0, bool verbose = false);
        Hostname hostname() const;
        Service service() const;
        Result result() const;

    private:
        int code;
        std::string error;
        Buffer host;
        Buffer serv;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Address& address);
    extern Endpoint to_endpoint(const Address& address,
                                bool numeric = false,
                                bool verbose = false);
    extern Hostname to_hostname(const Address& address,
                                bool verbose = false);
    extern Hostname to_string(const Address& address,
                              bool verbose = false);
}

#endif
