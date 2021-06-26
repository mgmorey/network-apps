#include "network-unique.h"     // Unique, operator<<()

#include <cassert>      // assert()

Network::Unique::Unique(const std::vector<std::size_t>& counts,
                        const std::string& noun) :
    item_counts(counts),
    item_noun(noun)
{
    assert(item_counts.size() == 2);
}

std::size_t Network::Unique::delta() const
{
    assert(item_counts[0] >= item_counts[1]);
    return item_counts[0] - item_counts[1];
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Unique& unique)
{
    if (unique.item_counts[0]) {
        std::size_t delta = unique.delta();
        
        os << unique.item_counts[0] << ' '
           << (unique.item_noun.empty() ? "items" :
               unique.item_noun);

        if (delta) {
            os << " ("
               << unique.item_counts[1]
               << " unique + "
               << delta
               << " duplicate)";
        }
    }

    return os;
}
