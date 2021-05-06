#include <cassert>
#include <fpcpp/datatypes/io.hpp>
#include <iostream>

int main() {
  using namespace fpcpp;
  const auto io_one = io::of(1);
  // Functor Check
  constexpr const auto identity_func = [](const auto x) { return x; };
  // We can't compare two std::function objects directly
  assert(io_one.map(identity_func).unsafePerform() == io_one.unsafePerform());

  constexpr const auto f = [](const auto x) { return x + 1; };
  constexpr const auto g = [](const auto x) { return x + 2; };
  assert(io_one.map([f, g](const auto x) { return f(g(x)); }).unsafePerform() ==
         io_one.map(g).map(f).unsafePerform());
  // Apply Check
  constexpr const auto plus_one = [](const auto x) { return x + 1; };
  constexpr const auto plus_two = [](const auto x) { return x + 2; };
  constexpr const auto h = [](const auto f_) {
    return [f_](const auto g_) {
      return [f_, g_](const auto x) { return f_(g_(x)); };
    };
  };
  const auto io_1 = io::of(plus_one);
  const auto io_2 = io::of(plus_two);
  const auto io_3 = io::of(3);
  assert(io_3.ap(io_2.ap(io_1.map(h))).unsafePerform() ==
         io_3.ap(io_2).ap(io_1).unsafePerform());
  // Applicative Check
  assert(io_3.ap(io_2.of(identity_func)).unsafePerform() ==
         io_3.unsafePerform());
  assert(io_3.of(4).ap(io_2.of(plus_one)).unsafePerform() ==
         io_3.of(plus_one(4)).unsafePerform());
  assert(io_3.of(4).ap(io_2).unsafePerform() ==
         io_2.ap(io_3.of([](const auto f_) { return f_(4); })).unsafePerform());
  // Chain Check
  constexpr const auto f_m = [](const auto x) { return io::of(x); };
  constexpr const auto g_m = [](const auto x) { return io::of(x + 1); };
  assert(io_3.chain(f_m).chain(g_m).unsafePerform() ==
         io_3.chain([f_m, g_m](const auto x) { return f_m(x).chain(g_m); })
             .unsafePerform());
  // Monad Check
  assert(io_3.of(1).chain(f_m).unsafePerform() == f_m(1).unsafePerform());
  assert(
      io_3.chain([io_2](const auto x) { return io_2.of(x); }).unsafePerform() ==
      io_3.unsafePerform());
}
