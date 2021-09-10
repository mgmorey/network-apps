#ifndef NETWORK_INTEGER_H
#define NETWORK_INTEGER_H

namespace Network
{
    class Integer
    {
    public:
        explicit Integer(int t_value);
        Integer& operator=(int t_value);
        explicit operator int() const;

    protected:
        int m_value;
    };
}

#endif
