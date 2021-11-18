#ifndef NETWORK_HINTS_H
#define NETWORK_HINTS_H

#include "network/family.h"         // Family, operator<<()
#include "network/flags.h"          // Flags, operator<<()
#include "network/protocol.h"       // Protocol, operator<<()
#include "network/socktype.h"       // SockType, operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

namespace Network
{
    struct Hints
    {
        explicit Hints(int t_family = 0,
                       int t_socktype = 0,
                       int t_protocol = 0,
                       int t_flags = 0);
        // cppcheck-suppress noExplicitConstructor
        Hints(const addrinfo& t_addrinfo);  // NOLINT
        Hints& operator=(const addrinfo& t_addrinfo);
        bool operator<(const Hints& t_hints) const;
        bool operator>(const Hints& t_hints) const;
        bool operator==(const Hints& t_hints) const;
        operator addrinfo() const;  // NOLINT
        [[nodiscard]] Flags flags() const;
        [[nodiscard]] Family family() const;
        [[nodiscard]] SockType socktype() const;
        [[nodiscard]] Protocol protocol() const;

    private:
        Integer<flags_type> m_flags;
        Integer<family_type> m_family;
        Integer<socktype_type> m_socktype;
        Integer<protocol_type> m_protocol;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Hints& hints);
}

#endif
