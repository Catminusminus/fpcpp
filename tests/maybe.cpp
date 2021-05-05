#include <iostream>
#include <fpcpp/datatypes/maybe.hpp>

int main()
{
    using namespace fpcpp;
    constexpr const auto maybe_one = maybe::of(1);
    constexpr const auto nothing = maybe::Nothing<int>();
    // Functor Check
    constexpr const auto identity_func = [](const auto x) { return x; };
    static_assert(maybe_one.map(identity_func) == maybe_one);
    static_assert(nothing.map(identity_func) == nothing);

    constexpr const auto f = [](const auto x) { return x + 1; };
    constexpr const auto g = [](const auto x) { return x + 2; };
    static_assert(maybe_one.map([f, g](const auto x) { return f(g(x)); }) == maybe_one.map(g).map(f));
    // Apply Check
    constexpr const auto plus_one = [](const auto x) { return x + 1; };
    constexpr const auto plus_two = [](const auto x) { return x + 2; };
    constexpr const auto h = [](const auto f_) {
        return [f_](const auto g_) {
            return [f_, g_](const auto x) {
                return f_(g_(x));
            };
        };
    };
    constexpr const auto maybe_1 = maybe::of(plus_one);
    constexpr const auto maybe_2 = maybe::of(plus_two);
    constexpr const auto maybe_3 = maybe::of(3);
    static_assert(maybe_3.ap(maybe_2.ap(maybe_1.map(h))) == maybe_3.ap(maybe_2).ap(maybe_1));
}
