#ifndef FPCPP_DATATYPES_STATET_HPP
#define FPCPP_DATATYPES_STATET_HPP

#include <type_traits>
#include <fpcpp/concepts/concepts.hpp>

namespace fpcpp
{
    template <class S, Monad M, class A>
    class StateT
    {
    public:
        using value_type = T;

    private:
        value_type value;

    public:
        constexpr Identity(const T &value) : value(value) {}
        template <class S>
        static constexpr auto of(const S &value) noexcept
        {
            return Identity(value);
        }
        template <class F>
        constexpr auto map(const F &function) const noexcept(noexcept(function(value)))
        {
            return Identity(function(value));
        }
        template <class F>
        constexpr auto ap(const Identity<F> &identity) const noexcept(noexcept(identity.value(value)))
        {
            return Identity(identity.value(value));
        }
        template <class F>
        constexpr auto chain(const F &function) const noexcept(noexcept(function(value)))
        {
            return function(value);
        }
        constexpr auto operator==(const Identity<T> &that) const noexcept
        {
            return value == that.value;
        }
    };

    template <class T>
    concept identity = std::is_same_v<T, Identity<typename T::value_type>>;
}

#endif // FPCPP_DATATYPES_STATET_HPP
