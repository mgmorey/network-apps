#ifndef NETWORK_FAMILY_H
#define NETWORK_FAMILY_H

#include <ostream>      // std::ostream

namespace Network
{
    class Family
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Family& family);

    public:
        Family(int family);

    private:
        int value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Family& family);
}

#endif
