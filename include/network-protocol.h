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
        Protocol(int t_family, int t_value);

    private:
        int m_family;
        int m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Protocol& protocol);
}

#endif
