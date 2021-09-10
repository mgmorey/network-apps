#ifndef NETWORK_INTEGER_H
#define NETWORK_INTEGER_H

namespace Network
{
    class Integer
    {
    public:
        explicit Integer(int t_value);
        Integer& operator=(int t_value);
        bool operator<(const Integer& t_integer) const;
        bool operator>(const Integer& t_integer) const;
        bool operator==(const Integer& t_integer) const;
        explicit operator int() const;

    protected:
        int m_value;
    };
}

#endif
