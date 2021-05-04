#include <iostream>
#include <fpcpp/datatypes/maybe.hpp>

int main()
{
    using namespace fpcpp;
    constexpr const auto nothing = Nothing();
    constexpr const auto just_3 = Just(3);
    static_assert(Nothing::of(0) == nothing);
    static_assert(Just<int>::of(0) == Just(0));

    constexpr const auto identity = [](auto x)
    { return x; };
    static_assert(Nothing::of(0).map(identity) == nothing);

    constexpr const auto maybe_identity = []<maybe T>(const T &x)
    { return x; };
    static_assert(maybe_identity(nothing) == nothing);
    static_assert(maybe_identity(just_3) == just_3);
}
