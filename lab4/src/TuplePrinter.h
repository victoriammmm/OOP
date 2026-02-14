#pragma once

#include <tuple>
#include <ostream>
#include <utility>

template<std::size_t N>
struct TuplePrinter {
    template<typename Ch, typename Tr, typename... Args>
    static void print(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
        TuplePrinter<N - 1>::print(os, t);
        os << ", " << std::get<N>(t);
    }
};

template<>
struct TuplePrinter<0> {
    template<typename Ch, typename Tr, typename... Args>
    static void print(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
        os << std::get<0>(t);
    }
};

template<typename Ch, typename Tr, typename... Args>
auto operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t)
    -> std::basic_ostream<Ch, Tr>& {

    os << "(";
    if constexpr (sizeof...(Args) > 0) {
        TuplePrinter<sizeof...(Args) - 1>::print(os, t);
    }
    os << ")";
    return os;
}