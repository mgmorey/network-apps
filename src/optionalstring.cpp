#include "network/optionalstring.h" // OptionalString

Network::OptionalString::OptionalString(const std::nullopt_t& t_value)
{
    static_cast<void>(t_value);
}

Network::OptionalString::OptionalString(const std::string& t_value) :
    m_value(t_value)
{
}

Network::OptionalString::OptionalString(const char* t_value)
{
    if (t_value != nullptr) {
        m_value = t_value;
    }
}

Network::OptionalString&
Network::OptionalString::operator=(const std::nullopt_t& t_value)
{
    static_cast<void>(t_value);
    m_value.reset();
    return *this;
}

Network::OptionalString&
Network::OptionalString::operator=(const std::string& t_value)
{
    m_value = t_value;
    return *this;
}

Network::OptionalString&
Network::OptionalString::operator=(const char* t_value)
{
    m_value.reset();

    if (t_value != nullptr) {
        m_value = t_value;
    }

    return *this;
}

Network::OptionalString::operator std::string() const
{
    return m_value.value_or("");
}

Network::OptionalString::operator const char*() const
{
    return m_value.has_value() ? m_value.value().c_str() : nullptr;
}

bool Network::OptionalString::has_value() const
{
    return m_value.has_value();
}

std::string::size_type Network::OptionalString::size() const
{
    return m_value.has_value() ? m_value.value().size() : 0;
}

std::string Network::OptionalString::value() const
{
    return m_value.value_or("");
}

std::string Network::OptionalString::value_or(const std::string& t_value) const
{
    return m_value.value_or(t_value);
}
