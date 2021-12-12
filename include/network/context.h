#ifndef NETWORK_CONTEXT_H
#define NETWORK_CONTEXT_H

#include "network/result.h"         // Result

#ifdef _WIN32
#include <winsock2.h>       // MAKEWORD(), WORD, WSADATA
#endif

namespace Network
{
    class Context
    {
    public:
        Context() = delete;
        Context(const Context&) = delete;
        Context(const Context&&) = delete;
        explicit Context(bool t_verbose = false);
        ~Context();
        auto operator=(const Context&) -> Context& = delete;
        auto operator=(const Context&&) -> Context& = delete;
        [[nodiscard]] auto result() const -> Result;

    private:
#ifdef _WIN32
        static constexpr WORD m_version {MAKEWORD(2, 2)};
        static unsigned m_count;
        static WSADATA m_data;
#endif

        Result m_result;
    };
}

#endif
