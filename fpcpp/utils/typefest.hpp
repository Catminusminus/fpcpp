#ifndef FPCPP_UTILS_TYPEFEST_HPP
#define FPCPP_UTILS_TYPEFEST_HPP

#include <type_traits>

namespace fpcpp
{
    template <class, class = void>
    struct has_value_type : std::false_type
    {
    };
    template <class S>
    struct has_value_type<S, std::void_t<typename S::value_type>> : std::true_type
    {
    };
    template <class S>
    constexpr auto has_value_type_v = has_value_type<S>::value;
}

#endif // FPCPP_UTILS_TYPEFEST_HPP
