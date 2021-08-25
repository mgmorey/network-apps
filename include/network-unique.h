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
        typedef std::vector<std::size_t> Counts;

        Unique(const Counts& m_counts,
               const std::string& m_noun = "");

    private:
        std::size_t delta() const;

        Counts m_counts;
        std::string m_noun;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Unique& unique);
}

#endif
