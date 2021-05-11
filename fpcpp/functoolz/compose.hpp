#ifndef FPCPP_FUNCTOOLZ_COMPOSE_HPP
#define FPCPP_FUNCTOOLZ_COMPOSE_HPP

#include <utility>

namespace fpcpp
{
    template <class Head, class ...Tails>
    constexpr auto compose(Head&& head, Tails&& ...tails)
    {
        // Todo: fix this to use perfect forwarding
        return [head, tails...](auto&& ...args) {
            return std::forward<Head>(head)(std::forward<Tails>(tails)(std::forward<decltype(args)>(args)...)...);
        };
    }
}

#endif // FPCPP_FUNCTOOLZ_COMPOSE_HPP
