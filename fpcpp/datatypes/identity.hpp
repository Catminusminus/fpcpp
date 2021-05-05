#ifndef FPCPP_DATATYPES_IDENTITY_HPP
#define FPCPP_DATATYPES_IDENTITY_HPP

#include <type_traits>

namespace fpcpp
{
    template <class T>
    class Identity
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
}

#endif // FPCPP_DATATYPES_IDENTITY_HPP
