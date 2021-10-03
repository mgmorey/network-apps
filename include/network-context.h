#ifndef NETWORK_CONTEXT_H
#define NETWORK_CONTEXT_H

#include "network-result.h"     // Result

#ifdef _WIN32
#include <winsock2.h>   // MAKEWORD(), WSADATA
#endif

#include <cstddef>      // std::size_t

namespace Network
{
    class Context
    {
    public:
        Context(bool t_verbose = false);
        ~Context();
        Result result();

    private:
        static constexpr WORD m_version {MAKEWORD(2, 2)};
        static std::size_t m_count;
#ifdef _WIN32
        static WSADATA m_data;
#endif

        Result m_result;
    };
}

#endif
