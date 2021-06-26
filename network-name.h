#ifndef NETWORK_NAME_H
#define NETWORK_NAME_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Name
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Name& name);

    public:
        Name(const std::string& name);
        Name(const char* name);

    private:
        bool null;
        const std::string value;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Name& name);
}

#endif
