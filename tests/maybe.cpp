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
    // Applicative Check
    static_assert(maybe_3.ap(maybe_2.of(identity_func)) == maybe_3);
    static_assert(maybe_3.of(4).ap(maybe_2.of(plus_one)) == maybe_3.of(plus_one(4)));
    static_assert(maybe_3.of(4).ap(maybe_2) == maybe_2.ap(maybe_3.of([](const auto f_) { return f_(4); })));
    // Chain Check
    constexpr const auto f_m = [](const auto x) { return maybe::of(x); };
    constexpr const auto g_m = [](const auto x) { return maybe::of(x + 1); };
    static_assert(maybe_3.chain(f_m).chain(g_m) == maybe_3.chain([f_m, g_m](const auto x) { return f_m(x).chain(g_m); }));
    // Monad Check
    static_assert(maybe_3.of(1).chain(plus_one) == plus_one(1));
    static_assert(maybe_3.chain([maybe_2](const auto x) { return maybe_2.of(x); }) == maybe_3);
}
