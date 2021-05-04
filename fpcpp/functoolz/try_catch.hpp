#ifndef FPCPP_FUNCTOOLZ_TRY_CATCH_HPP
#define FPCPP_FUNCTOOLZ_TRY_CATCH_HPP

#include <fpcpp/datatypes/maybe.hpp>
#include <fpcpp/datatypes/either.hpp>

namespace fpcpp
{
    template <class F>
    constexpr auto try_catch(const F &function)
    {
        try
        {
            return Right(function());
        }
        catch (const std::exception &e)
        {
            return Left(e);
        }
    }

    template <class F>
    constexpr auto try_catch_maybe(const F &function)
    {
        try
        {
            return Just(function());
        }
        catch ()
        {
            return Nothing();
        }
    }
}

#endif // FPCPP_FUNCTOOLZ_TRY_CATCH_HPP
