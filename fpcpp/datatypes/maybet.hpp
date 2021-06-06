#ifndef FPCPP_DATATYPES_MAYBET_HPP
#define FPCPP_DATATYPES_MAYBET_HPP

#include <type_traits>
#include <fpcpp/utils/typefest.hpp>
#include <fpcpp/concepts/concepts.hpp>
#include <fpcpp/datatypes/maybe.hpp>

namespace fpcpp
{
    template <template <class C> class M, class T>
    class MaybeT
    {
    public:
        using value_type = T;

    private:
        M<Maybe<T>> value;

    public:
        constexpr MaybeT(const M<Maybe<T>> &value) : value(value) {}
        constexpr MaybeT(M<Maybe<T>> &&value) : value(std::move(value)) {}
        template <template <class D> class N, class S>
        static constexpr auto of(const S &value)
        {
            return MaybeT<N, S>(N<Maybe<S>>::of(maybe::of(value)));
        }
        constexpr auto runMaybeT() const noexcept
        {
            return value;
        }
        template <class F>
        constexpr auto map(const F &function) const
        {
            using return_type = std::invoke_result_t<F, T>;
            return MaybeT<M, return_type>(value.map([function](const auto x) { return x.map(function); }));
        }
        template <class F>
        constexpr auto chain(const F &function) const
        {
            return value.chain([function](const auto x) { return x.chain(function); });
        }
        template <class S>
        constexpr auto ap(const MaybeT<M, S> &that) const noexcept
        {
            return value.ap(that.map([](const Maybe<S>& f) { 
                return [f](const Maybe<T>& maybe) {
                    return maybe.ap(f);
                }
            }));
        }
        constexpr auto operator==(const MaybeT<M, T> &that) const noexcept
        {
            return value == that.value;
        }
    };

    namespace maybe
    {
        template <template <class C> class M, class T>
        constexpr auto of(T &&value) noexcept
        {
            return MaybeT<M, T>::of(std::forward<T>(value));
        }
    }
}

#endif // FPCPP_DATATYPES_MAYBET_HPP
