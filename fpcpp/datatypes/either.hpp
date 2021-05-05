#ifndef FPCPP_DATATYPES_EITHER_HPP
#define FPCPP_DATATYPES_EITHER_HPP

#include <type_traits>
#include <variant>

namespace fpcpp
{
    template <class E, class T>
    class Either
    {
    public:
        using value_type = T;
        using right_type = T;
        using error_type = E;
        using left_type = E;

    private:
        std::variant<error_type, value_type> wrapped_value;

        template <class F>
        constexpr auto check_function_can_throw_exceptions(const F &function) const
        {
            if (wrapped_value.index() == 0)
            {
                return;
            }
            function(std::get<1>(wrapped_value));
        }

    public:
        constexpr Either(const E &error) : wrapped_value(error) {}
        constexpr Either(E &&error) : wrapped_value(std::move(error)) {}
        constexpr Either(const T &value) : wrapped_value(value) {}
        constexpr Either(T &&value) : wrapped_value(std::move(value)) {}
        constexpr Either(const std::variant<E, T> &wrapped_value) : wrapped_value(wrapped_value) {}
        constexpr Either(std::variant<E, T> &&wrapped_value) : wrapped_value(std::move(wrapped_value)) {}
        template <class E2, class T2>
        static constexpr auto of(const T2 &value) noexcept
        {
            return Either<E2, T2>(value);
        }
        constexpr auto unwrap() const noexcept
        {
            return wrapped_value;
        }
        constexpr auto is_left() const noexcept
        {
            return wrapped_value.index() == 0;
        }
        template <class F>
        constexpr auto map(const F &function) const noexcept(noexcept(check_function_can_throw_exceptions(function)))
        {
            using return_type = std::invoke_result_t<F, T>;
            if (!(this->is_left()))
            {
                return Either<E, return_type>(function(std::get<1>(wrapped_value)));
            }
            return Either<E, return_type>(wrapped_value);
        }
        template <class F>
        constexpr auto chain(const F &function) const noexcept(noexcept(check_function_can_throw_exceptions(function)))
        {
            using return_type = std::invoke_result_t<F, T>;
            std::variant<error_type, return_type> return_value;
            if (!(this->is_left()))
            {
                return_value.emplace(std::get<1>(wrapped_value));
                return return_value;
            }
            return_value.emplace(std::get<0>(wrapped_value));
            return return_value;
        }
        template <class E, class F>
        constexpr auto ap(const Either<E, F> &that) const noexcept
        {
            using return_type = std::invoke_result_t<F, T>;
            if (that->is_left())
            {
                return Either<E, return_type>(that.unwrap());
            }
            if (is_left())
            {
                return Either<E, return_type>(unwrap());
            }
            return Either<E, return_type>(std::get<1>(that.unwrap())(std::get<1>(unwrap())));
        }
        constexpr auto operator==(const Either<E, T> &that) const noexcept
        {
            return wrapped_value == that.wrapped_value;
        }
    };

    namespace either
    {
        template <class E, class T>
        constexpr auto of(T &&value) noexcept
        {
            return Either<E, T>(std::forward<T>(value));
        }
        template <class T, class E>
        constexpr auto Left(E &&error) noexcept
        {
            return of<T>(std::forward<E>(value));
        }
        template <class T, class E>
        constexpr auto Right(T &&value) noexcept
        {
            return of<E>(std::forward<T>(value));
        }
    }
}

#endif // FPCPP_DATATYPES_EITHER_HPP
