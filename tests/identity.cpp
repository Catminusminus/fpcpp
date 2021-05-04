#include <fpcpp/datatypes/identity.hpp>

int main()
{
    using namespace fpcpp;
    constexpr const auto identity = Identity<int>::of(1);
    // Functor Check
    constexpr const auto identity_func = [](const auto x)
    { return x; };
    static_assert(identity.map(identity_func) == identity);
}
