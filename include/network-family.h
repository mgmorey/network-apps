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
        explicit Family(int t_value);
        Family& operator=(int t_value);
        explicit operator int() const;

    private:
        int m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Family& family);
}

#endif
