#ifndef FPCPP_CONCEPTS_CONCEPTS_HPP
#define FPCPP_CONCEPTS_CONCEPTS_HPP

namespace fpcpp
{

    template <class T>
    concept Functor = requires(T x)
    {
        T::map;
    };

    template <class T>
    concept Apply = requires(T x)
    {
        requires Functor<T>;
        T::ap;
    };

    template <class T>
    concept Applicative = requires(T x)
    {
        requires Apply<T>;
        T::of;
    };

    template <class T>
    concept Chain = requires(T x)
    {
        requires Apply<T>;
        T::chain;
    };

    template <class T>
    concept Monad = requires(T x)
    {
        requires Applicative<T>;
        requires Chain<T>;
    };

}

#endif // FPCPP_CONCEPTS_CONCEPTS_HPP
