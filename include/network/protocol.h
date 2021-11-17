#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include "network/family.h"         // Family
#include "network/integer.h"        // Integer

#include <ostream>      // std::ostream

namespace Network
{
    class Protocol :
        public Integer<unsigned short>
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Protocol& protocol);

    public:
        Protocol(Family t_family, unsigned short t_value);
        Protocol& operator=(unsigned short t_value);
        Family family() const;

    private:
        Family m_family;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Protocol& protocol);
}

#endif
