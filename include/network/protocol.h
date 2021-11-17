#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include "network/family.h"         // Family
#include "network/integer.h"        // Integer
#include "network/socketargument.h" // socket_argument_type

#include <ostream>      // std::ostream

namespace Network
{
    using protocol_type = socket_argument_type;

    class Protocol :
        public Integer<protocol_type>
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Protocol& protocol);

    public:
        Protocol(Family t_family, protocol_type t_value);
        Protocol& operator=(protocol_type t_value);
        Family family() const;

    private:
        Family m_family;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Protocol& protocol);
}

#endif
