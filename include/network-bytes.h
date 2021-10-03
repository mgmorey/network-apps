#ifndef NETWORK_BYTES_H
#define NETWORK_BYTES_H

#include <cstddef>      // std::byte, std::size_t
#include <ostream>      // std::ostream
#include <string>       // std::string
#include <vector>       // std::vector

namespace Network
{
    class Bytes
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Bytes& bytes);
        friend std::string to_string(const Bytes& bytes);

    public:
        Bytes();
        Bytes(const std::byte* first, const std::byte* last);
        std::byte* data();
        const std::byte* data() const;
        bool empty() const;
        std::size_t size() const;

    private:
        std::vector<std::byte> m_bytes;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Bytes& bytes);
    extern std::string to_string(const Bytes& bytes);
}

#endif
