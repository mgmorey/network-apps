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
        auto operator=(const addrinfo& t_addrinfo) -> Hints&;
        auto operator<(const Hints& t_hints) const -> bool;
        auto operator>(const Hints& t_hints) const -> bool;
        auto operator==(const Hints& t_hints) const -> bool;
        operator addrinfo() const;  // NOLINT
        [[nodiscard]] auto flags() const -> Flags;
        [[nodiscard]] auto family() const -> Family;
        [[nodiscard]] auto socktype() const -> SockType;
        [[nodiscard]] auto protocol() const -> Protocol;

    private:
        Integer<flags_type> m_flags;
        Integer<family_type> m_family;
        Integer<socktype_type> m_socktype;
        Integer<protocol_type> m_protocol;
    };

    extern auto operator<<(std::ostream& os,
                           const Hints& hints) ->  std::ostream&;
}

#endif
