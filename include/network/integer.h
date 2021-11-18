#ifndef NETWORK_INTEGER_H
#define NETWORK_INTEGER_H

namespace Network
{
    template<typename T>
    class Integer
    {
    public:
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE
        Integer(T t_value) :
            m_value(t_value)
        {
        }

        Integer& operator=(T t_value)
        {
            m_value = t_value;
            return *this;
        }

        bool operator<(const Integer& t_integer) const
        {
            return (m_value < t_integer.m_value);
        }

        bool operator>(const Integer& t_integer) const
        {
            return (m_value > t_integer.m_value);
        }

        bool operator==(const Integer& t_integer) const
        {
            return (m_value == t_integer.m_value);
        }

        operator T() const  /// NOLINT
        {
            return m_value;
        }

    private:
        T m_value {0};
    };
}

#endif
