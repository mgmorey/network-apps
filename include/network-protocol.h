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
        Protocol(int protocol);

    private:
        int value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Protocol& protocol);
}

#endif
