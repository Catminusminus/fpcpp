#ifndef FPCPP_DATATYPES_IO_HPP
#define FPCPP_DATATYPES_IO_HPP

#include <type_traits>
#include <fpcpp/utils/typefest.hpp>

namespace fpcpp
{
    template <class A>
    class IO
    {
    public:
        using value_type = A;
        using action_type = std::function<A()>;

    private:
        action_type action;

    public:
        IO(const action_type &action) : action(action) {}
        IO(action_type &&action) : action(std::move(action)) {}
        auto unsafePerform()
        {
            return action();
        }
        template <class B>
        static auto of(const B &action) noexcept
        {
            return IO(action);
        }
        template <class F>
        auto map(const F &function) const noexcept
        {
            using return_type = std::invoke_result_t<F, A>;
            return IO<return_type>([] { return function(action()); });
        }
        template <class F>
        auto chain(const F &function) const noexcept
        {
            using return_type = std::invoke_result_t<F, A>;
            if constexpr (!has_value_type_v<return_type>)
            {
                return chain_not_return_io(function);
            }
            else if constexpr (std::is_same_v<return_type, IO<typename return_type::value_type>>)
            {
                return chain_return_io(function);
            }
            else
            {
                return chain_not_return_io(function);
            }
        }
        template <class F>
        auto chain_return_io(const F &function) const noexcept
        {
            using return_type = std::invoke_result_t<F, A>;
            return IO<typename return_type::value_type>([] { return function(action()).unsafePerform(); });
        }
        template <class F>
        auto chain_not_return_io(const F &function) const noexcept
        {
            return [] { return function(action()); };
        }
        template <class F>
        constexpr auto ap(const IO<F> &that) const noexcept
        {
            using return_type = std::invoke_result_t<F, A>;
            return IO<return_type>([] { return that.unsafePerform()(action()); });
        }
        constexpr auto operator==(const IO<A> &that) const noexcept
        {
            return action == that.action;
        }
    };
}

#endif // FPCPP_DATATYPES_IO_HPP
