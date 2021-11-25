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
        [[nodiscard]] result_type result() const;
        [[nodiscard]] std::string string() const;

    private:
        result_type m_result {0};
        std::string m_string;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Result& result);
}

#endif
