#ifndef NETWORK_RESULT_H
#define NETWORK_RESULT_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    using result_type = long long;

    struct Result
    {
        Result() = default;
        Result(result_type t_result, std::string t_string);
        operator bool() const;  // NOLINT
        [[nodiscard]] auto result() const -> result_type;
        [[nodiscard]] auto string() const -> std::string;

    private:
        result_type m_result {0};
        std::string m_string;
    };

    extern auto operator<<(std::ostream& os,
                           const Result& result) -> std::ostream&;
}

#endif
