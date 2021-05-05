#ifndef FPCPP_DATATYPES_MAYBE_HPP
#define FPCPP_DATATYPES_MAYBE_HPP

#include <type_traits>
#include <optional>

namespace fpcpp
{
    template <class T>
    class Maybe
    {
    public:
        using value_type = T;

    private:
        std::optional<T> wrapped_value;

        template <class F>
        constexpr auto check_function_can_throw_exceptions(const F &function) const
        {
            if (!wrapped_value)
            {
                return;
            }
            function(wrapped_value.value());
        }

    public:
        constexpr Maybe() {}
        constexpr Maybe(const T &value) : wrapped_value(value) {}
        constexpr Maybe(T &&value) : wrapped_value(std::move(value)) {}
        constexpr Maybe(const std::optional<T> &wrapped_value) : wrapped_value(wrapped_value) {}
        constexpr Maybe(std::optional<T> &&wrapped_value) : wrapped_value(std::move(wrapped_value)) {}
        template <class S>
        static constexpr auto of(const S &value)
        {
            return Maybe<S>(value);
        }
        constexpr auto unwrap() const noexcept
        {
            return wrapped_value;
        }
        template <class S>
        constexpr auto is_nothing() const noexcept
        {
            return !wrapped_value;
        }
        template <class F>
        constexpr auto map(const F &function) const noexcept(noexcept(check_function_can_throw_exceptions(function)))
        {
            using return_type = std::invoke_result_t<F, T>;
            if (wrapped_value)
            {
                return Maybe<return_type>(function(wrapped_value.value()));
            }
            return Maybe<return_type>();
        }
        template <class F>
        constexpr std::optional<std::invoke_result_t<F, T>> chain(const F &function) const noexcept(noexcept(check_function_can_throw_exceptions(function)))
        {
            if (wrapped_value)
            {
                return function(wrapped_value.value());
            }
            return std::nullopt;
        }
        template <class S>
        constexpr auto ap(const Maybe<S> &that) const noexcept
        {
            using return_type = std::invoke_result_t<S, T>;
            if (is_nothing() || that.is_nothing())
            {
                return Maybe<return_type>();
            }
            return Maybe(that.unwrap()(wrapped_value.value()));
        }
        constexpr auto operator==(const Maybe<T> &that) const noexcept
        {
            return wrapped_value == that.wrapped_value;
        }
    };

    namespace maybe
    {
        template <class T>
        constexpr auto of(T &&value) noexcept
        {
            return Maybe(std::forward<T>(value));
        }
        template <class T>
        constexpr auto Just(T &&value) noexcept
        {
            return of(std::forward<T>(value));
        }
        template <class T>
        constexpr auto Nothing() noexcept
        {
            return Maybe<T>();
        }
    }
}

#endif // FPCPP_DATATYPES_MAYBE_HPP
