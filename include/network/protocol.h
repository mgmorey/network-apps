#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include "network/family.h"             // Family, family_type
#include "network/integer.h"            // Integer
#include "network/protocol-type.h"      // protocol_type

#include <ostream>      // std::ostream

namespace Network
{
    class Protocol :
        public Integer<protocol_type>
    {
        friend auto operator<<(std::ostream& os,
                               const Protocol& protocol) -> std::ostream&;

    public:
        Protocol(family_type t_family, protocol_type t_value);
        [[nodiscard]] auto family() const -> Family;

    private:
        Family m_family;
    };

    extern auto operator<<(std::ostream& os,
                           const Protocol& protocol) -> std::ostream&;
}

#endif
