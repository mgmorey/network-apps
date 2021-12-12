#ifndef NETWORK_BUFFER_H
#define NETWORK_BUFFER_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Buffer
    {
    public:
        explicit Buffer(std::string::size_type t_size);
        operator std::string() const;  // NOLINT
        auto data() -> char*;
        [[nodiscard]] auto size() const -> std::string::size_type;

    private:
        std::string m_value;
    };

    extern auto operator<<(std::ostream& os,
                           const Buffer& buffer) -> std::ostream&;
}

#endif
