#ifndef STREAM_CONTAINER_H
#define STREAM_CONTAINER_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    template <typename Container>
    std::ostream& print(std::ostream& os,
                        const Container& cont,
                        const std::string& before = "",
                        const std::string& after = "\n")
    {
        typename Container::const_iterator first(cont.begin());
        typename Container::const_iterator last(cont.end());

        for (typename Container::const_iterator it = first;
             it != last;
             ++it) {
            os << before
               << *it
               << after;
        }

        return os;
    }
}

#endif
