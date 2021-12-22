#include "network/address.h"            // Address, sockaddr,
                                        // sockaddr_un
#include "network/get-sun-length.h"     // get_sun_path_length()
#include "network/get-sun-pointer.h"    // get_sun_pointer()
#include "network/to-string.h"          // to_string()

#include <string>       // std::string

#ifndef _WIN32

auto Network::Address::sun() const -> const sockaddr_un*
{
    return get_sun_pointer(m_value);
}

auto Network::Address::sun_family() const -> Network::family_type
{
    return sun()->sun_family;
}

auto Network::Address::sun_length() const -> Network::Address::length_type
{
#ifdef HAVE_SOCKADDR_SA_LEN
    return sun()->sun_len;
#else
    return 0;
#endif
}

auto Network::Address::sun_path() const -> Network::Address::value_type
{
    const auto size {m_value.size()};
    const auto length {
        size < m_sun_path_offset ? 0 :
        get_sun_path_length(sun(), size)
    };
    const auto offset {m_sun_path_offset};
    return m_value.substr(offset, length);
}

auto Network::Address::sun_text() const -> std::string
{
    const auto text {to_string(sun_path())};
    return text.substr(0, text.find('\0'));
}

#endif
