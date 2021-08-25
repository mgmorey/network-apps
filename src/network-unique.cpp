#include "network-unique.h"     // Unique, operator<<()

#include <cassert>      // assert()

Network::Unique::Unique(const Counts& t_counts,
                        const std::string& t_noun) :
    m_counts(t_counts),
    m_noun(t_noun)
{
    assert(m_counts.size() == 2);
}

std::size_t Network::Unique::delta() const
{
    assert(m_counts[0] >= m_counts[1]);
    return m_counts[0] - m_counts[1];
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Unique& unique)
{
    if (unique.m_counts[0]) {
        const std::size_t delta = unique.delta();

        os << unique.m_counts[0] << ' '
           << (unique.m_noun.empty() ? "items" :
               unique.m_noun);

        if (delta) {
            os << " ("
               << unique.m_counts[1]
               << " unique + "
               << delta
               << " duplicate)";
        }
    }

    return os;
}
