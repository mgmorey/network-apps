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
        explicit Flags(int t_value);
        Flags& operator=(int t_value);
        explicit operator int() const;

    private:
        int m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Flags& flags);
}

#endif
