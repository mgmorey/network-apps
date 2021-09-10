#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include "network-family.h"     // Family

#include <ostream>      // std::ostream

namespace Network
{
    class Protocol
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Protocol& protocol);

    public:
        Protocol(Family t_family, int t_value);
        Protocol& operator=(int t_value);
        explicit operator int() const;

    private:
        Family m_family;
        int m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Protocol& protocol);
}

#endif
