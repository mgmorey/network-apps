#include "network/address.h"            // Address, length_type,
                                        // sa_data_offset, sockaddr,
                                        // value_type
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sa-length.h"      // get_sa_length()
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/offsets.h"            // sa_data_offset
#include "network/print.h"              // print()

#include <sstream>      // std::ostringstream
#include <string>       // std::string

auto Network::Address::sa() const -> const sockaddr*
{
    return get_sa_pointer(m_value);
}

auto Network::Address::sa_data() const -> Network::Address::value_type
{
    return m_value.substr(sa_data_offset);
}

auto Network::Address::sa_family() const -> Network::family_type
{
    return get_sa_family(m_value);
}

auto Network::Address::sa_length() const -> Network::Address::length_type
{
    return get_sa_length(m_value);
}

auto Network::Address::sa_text() const -> std::string
{
    std::ostringstream oss;
    print(oss, m_value);
    return oss.str();
}
