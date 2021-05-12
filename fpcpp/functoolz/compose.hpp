#ifndef FPCPP_FUNCTOOLZ_COMPOSE_HPP
#define FPCPP_FUNCTOOLZ_COMPOSE_HPP

#include <utility>
#include <concepts>
#include <tuple>

namespace fpcpp
{
    template <class F>
    constexpr auto compose(F f)
    {
        return f;
    }
    template <class F, class G>
    constexpr auto compose(F f, G g)
    {
        return [=]<class ...Args>(Args&& ...args) {
            return f(g(std::forward<Args>(args)...));
        };
    }
    template <class F, class ...Fs>
    constexpr auto compose(F f, Fs ...fs)
    {
        return compose(f, compose(fs...));
    }
}

#endif // FPCPP_FUNCTOOLZ_COMPOSE_HPP
