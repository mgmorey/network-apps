#ifndef NETWORK_CONTEXT_H
#define NETWORK_CONTEXT_H

#include <cstddef>      // std::size_t

namespace Network
{
    class Context
    {
    public:
        Context();
        ~Context();

    private:
        static std::size_t m_count;
    };
}

#endif
