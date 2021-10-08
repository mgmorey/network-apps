#ifndef NETWORK_OPTIONAL_H
#define NETWORK_OPTIONAL_H

namespace Network
{
    class Optional
    {
    public:
        Optional()
        {
        }

        explicit Optional(bool t_null) :
            m_null(t_null)
        {
        }

        bool null() const
        {
            return m_null;
        }


    protected:
        bool m_null {true};
    };
}

#endif
