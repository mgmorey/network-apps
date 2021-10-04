#ifndef NETWORK_BYTESTR_H
#define NETWORK_BYTESTR_H

#include <cstddef>      // std::byte, std::size_t
#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class ByteString
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const ByteString& bytes);
        friend std::string to_string(const ByteString& bytes);

    public:
        ByteString();
        ByteString(const std::byte* first, const std::byte* last);
        const std::byte* cbegin(std::size_t t_offset = 0) const;
        const std::byte* cend(std::size_t t_offset = 0) const;
        const std::byte* data() const;
        bool empty() const;
        std::size_t size() const;

    private:
        std::basic_string<std::byte> m_bytes;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const ByteString& bytes);
    extern std::string to_string(const ByteString& bytes);
}

#endif
