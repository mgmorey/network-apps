#ifndef NETWORK_UNIQUE_H
#define NETWORK_UNIQUE_H

#include <cstddef>      // std::size_t
#include <ostream>      // std::ostream
#include <string>       // std::string
#include <vector>       // std::vector

namespace Network
{
    class Unique
    {
        friend std::ostream& operator<<(std::ostream& os, const Unique&);

    public:
        Unique(const std::vector<std::size_t>& counts,
               const std::string& noun = "");

    private:
        std::size_t delta() const;

        std::vector<std::size_t> item_counts;
        std::string item_noun;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Unique& unique);
}

#endif
