#include <fpcpp/functoolz/compose.hpp>

int main() {
    using namespace fpcpp;
    constexpr auto f = [](auto x) { return x + 1; };
    constexpr auto g = [](auto x) { return x + 2; };
    constexpr auto h = compose(f, g);
    static_assert(h(1) == f(g(1)));
}
