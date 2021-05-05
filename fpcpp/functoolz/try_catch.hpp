#ifndef FPCPP_FUNCTOOLZ_TRY_CATCH_HPP
#define FPCPP_FUNCTOOLZ_TRY_CATCH_HPP

#include <fpcpp/datatypes/maybe.hpp>
#include <fpcpp/datatypes/either.hpp>

namespace fpcpp
{
    struct WrappedError
    {
        std::exception error;
        constexpr WrappedError(const std::exception &error) : error(error) {}
    };

    template <class F>
    constexpr auto try_catch(const F &function) noexcept
    {
        using return_type = std::invoke_result_t<F>;
        try
        {
            return either::Right<WrappedError>(function());
        }
        catch (const std::exception &e)
        {
            return either::Left<return_type>(WrappedError(e));
        }
    }

    template <class F>
    constexpr auto try_catch_maybe(const F &function) noexcept
    {
        using return_type = std::invoke_result_t<F>;
        try
        {
            return maybe::Just(function());
        }
        catch ()
        {
            return maybe::Nothing<return_type>();
        }
    }
}

#endif // FPCPP_FUNCTOOLZ_TRY_CATCH_HPP
