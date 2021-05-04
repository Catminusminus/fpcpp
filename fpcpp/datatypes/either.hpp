#ifndef FPCPP_DATATYPES_EITHER_HPP
#define FPCPP_DATATYPES_EITHER_HPP

#include <type_traits>

namespace fpcpp
{
    template <class T>
    class Right;

    template <class T>
    class Left
    {
    public:
        using value_type = T;

    private:
        value_type value;

    public:
        constexpr Left(const T &value) : value(value) {}
        template <class S>
        static constexpr auto of(const S &value) noexcept
        {
            return Left(value);
        }
        template <class F>
        constexpr auto map(const F &) const noexcept
        {
            return (*this);
        }
        template <class S>
        constexpr auto ap(const Left<S> &left) const noexcept
        {
            return left;
        }
        template <class S>
        constexpr auto ap(const Right<S> &right) const noexcept
        {
            return (*this);
        }
        template <class F>
        constexpr auto chain(const F &) const noexcept
        {
            return (*this);
        }
        constexpr auto operator==(const Left<T> &that) const noexcept
        {
            return value == that.value;
        }
    };

    template <class T>
    class Right
    {
    public:
        using value_type = T;

    private:
        value_type value;

    public:
        constexpr Right(const T &value) : value(value) {}
        template <class S>
        static constexpr auto of(const S &value) noexcept
        {
            return Right(value);
        }
        template <class F>
        constexpr auto map(const F &function) const noexcept(noexcept(function(value)))
        {
            return Right(function(value));
        }
        template <class S>
        constexpr auto ap(const Left<S> &left) const noexcept
        {
            return left;
        }
        template <class F>
        constexpr auto ap(const Right<F> &right) const noexcept(noexcept(right.value(value)))
        {
            return Right(right.value(value));
        }
        template <class F>
        constexpr auto chain(const F &function) const noexcept(noexcept(function(value)))
        {
            return function(value);
        }
        constexpr auto operator==(const Right<T> &that) const noexcept
        {
            return value == that.value;
        }
    };

    template <class T>
    class Either
    {
    };

    template <class T>
    concept either = std::is_same_v<T, Left<typename T::value_type>> || std::is_same_v<T, Right<typename T::value_type>>;
}

#endif // FPCPP_DATATYPES_EITHER_HPP
