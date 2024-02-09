// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_STUPLE_HPP
#define YYMP_STUPLE_HPP

#include <cstddef>

#include <concepts>
#include <type_traits>
#include <utility>

#include <yymp/dtl/stuple_impl.hpp>
#include <yymp/wref_tuple.hpp>
#include <yymp/tuple_traits.hpp>

namespace yymp
{
    /**
     * \brief An aggregate tuple implementation.
     *
     * This implementation has no user-provided constructors, destructors, and
     * assignment operators. If all of \a Types are trivial, then the `stuple`
     * is also a trivial type.
     */
    template<typename... Types>
    struct stuple : dtl::stuple::make_impl<Types...> { };
    
    /**
     * \brief Deduction-guide for #stuple; value-oriented deduction.
     */
    template<typename... Types>
    stuple(Types...) -> stuple<Types...>; // value-oriented deduction
    
    // =========================================================================
    // The following specializations provide massive improvements in compile 
    // times when used in certain pathological cases.
    // These specializations drastically reduce the quadratic term involved in 
    // many chained stuple_cats.
    
    template<typename... Types>
    struct is_piecewise_reinitializable<const stuple<Types...>&>
        : ::std::is_copy_constructible<stuple<Types...>>::type { };
    
    template<typename... Types>
    struct is_piecewise_reinitializable<stuple<Types...>&&>
        : ::std::is_move_constructible<stuple<Types...>>::type { };
    
    template<typename... Types>
    struct is_piecewise_reinitializable<stuple<Types...>>
        : ::std::is_move_constructible<stuple<Types...>>::type { };
        
    template<typename... Types>
    struct is_piecewise_nothrow_reinitializable<const stuple<Types...>&>
        : ::std::is_nothrow_copy_constructible<stuple<Types...>>::type { };
    
    template<typename... Types>
    struct is_piecewise_nothrow_reinitializable<stuple<Types...>&&>
        : ::std::is_nothrow_move_constructible<stuple<Types...>>::type { };
    
    template<typename... Types>
    struct is_piecewise_nothrow_reinitializable<stuple<Types...>>
        : ::std::is_nothrow_move_constructible<stuple<Types...>>::type { };
    
    /**
     * \brief Converts a \a Tuple to its equivalent #stuple.
     *
     * The \a Tuple element types are determined by `std::tuple_element`.
     *
     * \param [in] t The tuple to convert.
     * \return The #stuple resulting from conversion.
     */
    template<piecewise_reinitializable Tuple>
    constexpr
    auto //stuple<...>
    to_stuple(Tuple&& t) noexcept(piecewise_nothrow_reinitializable<Tuple>)
    {
        using value_type = ::std::remove_cvref_t<Tuple>;
        
        return [&t] <::std::size_t... I> (::std::index_sequence<I...>)
        {
            using result_type = 
                stuple<typename ::std::tuple_element<I, value_type>::type...>;
            
            return result_type{get<I>(::std::forward<Tuple>(t))...};
        }(::std::make_index_sequence<::std::tuple_size<value_type>::value>{});
    }
    
    /**
     * \brief Extracts the elements of a `stuple`, mirroring `std::get`.
     *
     * This `using` declaration provides both indexed-based (`get<I>(...)`)
     * as well as type-based (`get<T>(...)`) accessors.
     */
    using dtl::stuple::get;
    
    /**
     * \brief Concatenates tuple-like objects into a #stuple.
     *
     * This function mirrors `std::tuple_cat` with the following exceptions:
     * * the return type is a #stuple specialization; and
     * * supports objects with a tuple-like protocol, extracting elements 
     *   via an `ADL`-located `get`.
     */
    template<piecewise_reinitializable... Tuples>
    constexpr
    auto // stuple<...> 
    stuple_cat(Tuples&&... args)
        noexcept((... && piecewise_nothrow_reinitializable<Tuples>))
    {
        return ::yymp::to_stuple(
            wref_tuple<Tuples&&...>{::std::forward<Tuples>(args)...}
        );
    }
    
    /**
     * \brief Concatenates two tuple-like objects into a #stuple.
     *
     * This function mirrors `std::tuple_cat` with the following exceptions:
     * * the return type is a #stuple specialization; and
     * * supports objects with a tuple-like protocol, extracting elements 
     *   via an `ADL`-located `get`.
     *
     * This overload exists to handle a common case and is an optimization with
     * meaningful results over the generic overload.
     */
    template<piecewise_reinitializable TupleA, piecewise_reinitializable TupleB>
    constexpr
    auto // stuple<...>
    stuple_cat(TupleA&& a, TupleB&& b)
        noexcept(
            piecewise_nothrow_reinitializable<TupleA> &&
            piecewise_nothrow_reinitializable<TupleB>
        )
    {
        using tuple_a = ::std::remove_cvref_t<TupleA>;
        using tuple_b = ::std::remove_cvref_t<TupleB>;

        return [&a, &b] <::std::size_t... AIs, ::std::size_t... BIs>
        (::std::index_sequence<AIs...>, ::std::index_sequence<BIs...>)
        {
            using result_type = stuple<
                typename ::std::tuple_element<AIs, tuple_a>::type...,
                typename ::std::tuple_element<BIs, tuple_b>::type...
            >;
            return result_type {
                get<AIs>(::std::forward<TupleA>(a))...,
                get<BIs>(::std::forward<TupleB>(b))...
            };
        }(::std::make_index_sequence<::std::tuple_size<tuple_a>::value>{},
          ::std::make_index_sequence<::std::tuple_size<tuple_b>::value>{});
    }
    
    /**
     * \brief Invokes \a f with the elements of \a t.
     *
     * This is a specialized overload of `yymp::apply` for `stuple`s that mirrors
     * `std::apply`.
     */
    template<typename F, typename... Types>
      requires ::std::invocable<F, const Types&...>
    constexpr decltype(auto) apply(F&& f, const stuple<Types...>& t)
      noexcept(::std::is_nothrow_invocable_v<F, const Types&...>)
    {
      return dtl::stuple::apply(::std::forward<F>(f), t);
    }
    
    /**
     * \brief Invokes \a f with the elements of \a t.
     *
     * This is a specialized overload of `yymp::apply` for `stuple`s that mirrors
     * `std::apply`.
     */
    template<typename F, typename... Types>
      requires ::std::invocable<F, Types&&...>
    constexpr decltype(auto) apply(F&& f, stuple<Types...>&& t)
      noexcept(::std::is_nothrow_invocable_v<F, Types&&...>)
    {
      return dtl::stuple::apply(::std::forward<F>(f), ::std::move(t));
    }
}

namespace std
{
    template<typename... Types>
    struct tuple_size<::yymp::stuple<Types...>>
        : std::integral_constant<
            std::size_t,
            sizeof...(Types)
        > { };
    
    template<std::size_t I, typename... Types>
    struct tuple_element<I, ::yymp::stuple<Types...>>
    {
        using type = decltype(
            ::yymp::dtl::stuple::deduce_type<I>(
                ::std::declval<::yymp::stuple<Types...>>()
            )
        );
    };
}

#endif // YYMP_STUPLE_HPP