#include "network/optionalstring.h" // OptionalString

Network::OptionalString::OptionalString(const std::string& t_string) :
    m_value(t_string)
{
}

Network::OptionalString::OptionalString(const char* t_value)
{
    m_value.reset();

    if (t_value != nullptr) {
        m_value = t_value;
    }
}

Network::OptionalString&
Network::OptionalString::operator=(const std::string& t_string)
{
    m_value = t_string;
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

std::string Network::OptionalString::value_or(const std::string& value) const
{
    return m_value.value_or(value);
}
