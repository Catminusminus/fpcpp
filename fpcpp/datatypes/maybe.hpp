#ifndef FPCPP_DATATYPES_MAYBE_HPP
#define FPCPP_DATATYPES_MAYBE_HPP

#include <type_traits>

namespace fpcpp
{
    class Nothing
    {

    public:
        constexpr Nothing() {}
        template <class T>
        static constexpr auto of(const T &value) noexcept
        {
            return Nothing();
        }
        template <class F>
        constexpr auto map(const F &) const noexcept
        {
            return Nothing();
        }
        template <class M>
        constexpr auto ap(const M &) const noexcept
        {
            return Nothing();
        }
        template <class F>
        constexpr auto chain(const F &) const noexcept
        {
            return Nothing();
        }
        constexpr auto operator==(const Nothing &that) const noexcept
        {
            return true;
        }
    };

    template <class T>
    class Just
    {
    public:
        using value_type = T;

    private:
        value_type value;

    public:
        constexpr Just(const T &value) : value(value) {}
        template <class S>
        static constexpr auto of(const S &value) noexcept
        {
            return Just(value);
        }
        template <class F>
        constexpr auto map(const F &function) const noexcept(noexcept(function(value)))
        {
            return Just(function(value));
        }
        constexpr auto ap(const Nothing &) const noexcept
        {
            return Nothing();
        }
        template <class F>
        constexpr auto ap(const Just<F> &just) const noexcept(noexcept(just.value(value)))
        {
            return Just(just.value(value));
        }
        template <class F>
        constexpr auto chain(const F &function) const noexcept(noexcept(function(value)))
        {
            return function(value);
        }
        constexpr auto operator==(const Just<T> &that) const noexcept
        {
            return value == that.value;
        }
    };

    template <class T>
    class Maybe
    {
    };

    template <class T>
    concept maybe = std::is_same_v<T, Nothing> || std::is_same_v<T, Just<typename T::value_type>>;
}

#endif // FPCPP_DATATYPES_MAYBE_HPP
