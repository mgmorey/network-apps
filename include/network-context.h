#ifndef NETWORK_CONTEXT_H
#define NETWORK_CONTEXT_H

#include "network-result.h"     // Result

#ifdef _WIN32
#include <winsock2.h>   // MAKEWORD(), WSADATA, WORD
#endif

#include <cstddef>      // std::size_t

namespace Network
{
    class Context
    {
    public:
        Context(bool t_verbose = false);
        ~Context();
        Result result() const;

    private:
#ifdef _WIN32
        static constexpr WORD m_version {MAKEWORD(2, 2)};
        static std::size_t m_count;
        static WSADATA m_data;
#endif

        Result m_result;
        bool m_verbose {false};
    };
}

#endif
