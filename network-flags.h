#ifndef NETWORK_FLAGS_H
#define NETWORK_FLAGS_H

#include <ostream>      // std::ostream

namespace Network
{
    class Flags
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Flags& flags);

    public:
        Flags(int flags);

    private:
        int value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Flags& flags);
}

#endif
