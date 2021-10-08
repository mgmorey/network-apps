#ifndef NETWORK_OPTIONAL_H
#define NETWORK_OPTIONAL_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Optional
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Optional& optional);

    public:
        Optional();
        // cppcheck-suppress noExplicitConstructor
        Optional(const std::string& t_optional);
        // cppcheck-suppress noExplicitConstructor
        Optional(const char* t_value);
        Optional& operator=(const std::string& t_optional);
        Optional& operator=(const char* t_value);
        operator std::string() const;
        operator const char*() const;
        bool empty() const;
        bool null() const;

    private:
        bool m_null;
        std::string m_value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Optional& optional);
}

#endif
