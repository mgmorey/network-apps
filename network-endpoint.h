#ifndef NETWORK_ENDPOINT_H
#define NETWORK_ENDPOINT_H

#include "network-address.h"    // Address
#include "network-buffer.h"     // Buffer
#include "network-result.h"     // Result
#include "network-types.h"      // Hostname, Service

#include <string>       // std::string

namespace Network
{
    class Endpoint
    {
    public:
        Endpoint(const Address& other, int flags = 0);
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
    extern Endpoint to_endpoint(const Address& other,
                                bool numeric = false);
    extern Hostname to_hostname(const Address& other,
                                bool numeric = false);
    extern Hostname to_string(const Address& other);
}

#endif
