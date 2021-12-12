#include "network/optionalstring.h" // OptionalString

Network::OptionalString::OptionalString(const std::nullopt_t& t_value) :
    m_value(t_value)
{
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

auto Network::OptionalString::operator=(const std::nullopt_t& t_value) ->
    Network::OptionalString&
{
    m_value = t_value;
    return *this;
}

auto Network::OptionalString::operator=(const std::string& t_value) ->
    Network::OptionalString&
{
    m_value = t_value;
    return *this;
}

auto Network::OptionalString::operator=(const char* t_value) ->
    Network::OptionalString&
{
    if (t_value == nullptr) {
        m_value.reset();
    }
    else {
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

auto Network::OptionalString::has_value() const -> bool
{
    return m_value.has_value();
}

auto Network::OptionalString::size() const -> std::string::size_type
{
    return m_value.has_value() ? m_value.value().size() : 0;
}

auto Network::OptionalString::value() const -> std::string
{
    return m_value.value_or("");
}

auto Network::OptionalString::value_or(const std::string& t_value) const ->
    std::string
{
    return m_value.value_or(t_value);
}
