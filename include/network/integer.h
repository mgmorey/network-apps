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

        auto operator=(T t_value) -> Integer&
        {
            m_value = t_value;
            return *this;
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
