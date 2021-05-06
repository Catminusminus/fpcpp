#ifndef FPCPP_DATATYPES_IO_HPP
#define FPCPP_DATATYPES_IO_HPP

#include <type_traits>
#include <functional>
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
        auto unsafePerform() const
        {
            return action();
        }
        template <class T>
        static auto of(const T &value) noexcept
        {
            return IO<T>([value]()
                         { return value; });
        }
        template <class F>
        auto map(const F &function) const noexcept
        {
            using return_type = std::invoke_result_t<F, A>;
            return IO<return_type>([function, *this]
                                   { return function(action()); });
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
            return IO<typename return_type::value_type>([function, *this]
                                                        { return function(action()).unsafePerform(); });
        }
        template <class F>
        auto chain_not_return_io(const F &function) const noexcept
        {
            return [function, *this]
            { return function(action()); };
        }
        template <class F>
        auto ap(const IO<F> &that) const noexcept
        {
            using return_type = std::invoke_result_t<F, A>;
            return IO<return_type>([that, *this]
                                   { return that.unsafePerform()(action()); });
        }
        auto operator==(const IO<A> &that) const noexcept
        {
            return action == that.action;
        }
    };

    namespace io
    {
        template <class T>
        auto of(T &&value) noexcept
        {
            return IO<T>([value = std::forward<T>(value)]
                         { return value; });
        }
    }
}

#endif // FPCPP_DATATYPES_IO_HPP
