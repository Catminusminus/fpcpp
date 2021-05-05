#include <iostream>
#include <fpcpp/datatypes/maybe.hpp>

int main()
{
    using namespace fpcpp;
    constexpr const auto maybe_one = maybe::of(1);
    constexpr const auto nothing = maybe::Nothing<int>();
    // Functor Check
    constexpr const auto identity_func = [](const auto x)
    { return x; };
    static_assert(maybe_one.map(identity_func) == maybe_one);
    static_assert(nothing.map(identity_func) == nothing);
}
