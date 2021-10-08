#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include "network-family.h"     // Family
#include "network-integer.h"    // Integer

#include <ostream>      // std::ostream

namespace Network
{
    class Protocol :
        public Integer<short>
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Protocol& protocol);

    public:
        Protocol(Family t_family, int t_value);
        Protocol& operator=(int t_value);
        Family family() const;

    private:
        Family m_family;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Protocol& protocol);
}

#endif
