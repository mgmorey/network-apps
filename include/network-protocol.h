#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include <ostream>      // std::ostream

namespace Network
{
    class Protocol
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Protocol& protocol);

    public:
        Protocol(int family, int protocol);

    private:
        int fam;
        int proto;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Protocol& protocol);
}

#endif
