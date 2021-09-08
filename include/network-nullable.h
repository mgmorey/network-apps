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
        explicit Nullable(const std::string& t_nullable);
        explicit Nullable(const char* t_value);
        Nullable& operator=(const std::string& t_nullable);
        Nullable& operator=(const char* t_value);
        operator std::string() const;
        const char* c_str() const;

    private:
        bool m_is_null;
        std::string m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Nullable& nullable);
}

#endif
