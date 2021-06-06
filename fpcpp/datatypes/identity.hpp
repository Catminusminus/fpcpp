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
        constexpr Identity(T &&value) : value(std::move(value)) {}
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
        static constexpr auto map(const F &function, const Identity& that) noexcept(noexcept(that.map(function)))
        {
            return that.map(function);
        }
        template <class F>
        constexpr auto ap(const Identity<F> &identity) const noexcept(noexcept(identity.value(value)))
        {
            return Identity(identity.value(value));
        }
        template <class F>
        static constexpr auto ap(const Identity<F> &identity, const Identity& that) noexcept(noexcept(that.ap(identity)))
        {
            return that.ap(identity);
        }
        template <class F>
        constexpr auto chain(const F &function) const noexcept(noexcept(function(value)))
        {
            return function(value);
        }
        template <class F>
        static constexpr auto chain(const F &function, const Identity& that) noexcept(noexcept(that.chain(function)))
        {
            return that.chain(function);
        }
        constexpr auto operator==(const Identity<T> &that) const noexcept
        {
            return value == that.value;
        }
    };

    namespace identity
    {
        template <class T>
        constexpr auto of(T &&value) noexcept
        {
            return Identity(std::forward<T>(value));
        }
        template <class F, class T>
        constexpr auto map(const F &function, const Identity<T>& that) noexcept(noexcept(that.map(function)))
        {
            return that.map(function);
        }
        template <class F, class T>
        constexpr auto ap(const Identity<F> &identity, const Identity<T>& that) noexcept(noexcept(that.ap(identity)))
        {
            return that.ap(identity);
        }
        template <class F, class T>
        constexpr auto chain(const F &function, const Identity<T>& that) noexcept(noexcept(that.chain(function)))
        {
            return that.chain(function);
        }
    }
}

#endif // FPCPP_DATATYPES_IDENTITY_HPP
