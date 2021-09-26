#ifndef NETWORK_CONTEXT_H
#define NETWORK_CONTEXT_H

#include "network-result.h"     // Result

#ifdef _WIN32
#include <winsock2.h>   // WSADATA
#endif

#include <cstddef>      // std::size_t

namespace Network
{
    class Context
    {
    public:
        Context();
        ~Context();
        Result result();

    private:
        static std::size_t m_count;
        Result m_result;
#ifdef _WIN32
        WSADATA m_data;
#endif
    };
}

#endif
