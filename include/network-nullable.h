#ifndef NETWORK_NULLABLE_H
#define NETWORK_NULLABLE_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Nullable
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Nullable& nullable);

    public:
        Nullable();
        Nullable(const std::string& other);
        Nullable(const char* other);
        Nullable& operator=(const std::string& other);
        Nullable& operator=(const char* other);
        operator std::string() const;

    private:
        bool value_is_null;
        std::string value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Nullable& nullable);
}

#endif
