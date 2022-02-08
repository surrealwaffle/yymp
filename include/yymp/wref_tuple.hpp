// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_WREF_TUPLE_HPP
#define YYMP_WREF_TUPLE_HPP

#include <cstddef>

#include <type_traits>
#include <utility>

#include <yymp/dtl/wref_tuple_impl.hpp>

namespace yymp
{
    /** 
     * \brief A tuple that provides a flattened view of multiple tuples.
     *
     * This template implements a tuple of tuple references - it is both 
     * wide and stores references, hence `wref_tuple`. This type also supports 
     * aggregate initialization, so temporaries may have their lifetime extended
     * through list initialization. Each subtuple reference has an implicit `&&`
     * which may collapse through normal reference collapsing rules.
     *
     * A replacement for `std::tuple_cat` is not provided; a similar operation 
     * can be performed by using another layer of `wref_tuple`, as shown in the 
     * snippet below.
     * \code {.cpp}
     * wref_tuple x = ...; // or another kind of tuple
     * wref_tuple y = ...; // or another kind of tuple
     * wref_tuple z {std::make_tuple(...), x, y}; // temporary lifetime extended
     * yymp::apply(function, z);
     * \endcode
     */
    template<typename... Tuples>
    struct wref_tuple : dtl::wref_tuple::make_impl<Tuples...> { };
    
    /**
     * \brief Deduction guide for `wref_tuple`; reference-oriented deduction.
     */
    template<typename... T>
    wref_tuple(T&&...) -> wref_tuple<T&&...>;

    /**
     * \brief Extracts the `I`th element out of a wide reference tuple \a t.
     *
     * This accessor respects the `cv`- and `ref`- qualifiers 
     * (i.e. `lvalue` or `rvalue` reference) of the binding subtuple reference.
     */
    template<::std::size_t I, typename... Tuples>
        requires (I < ::std::tuple_size<wref_tuple<Tuples...>>::value)
    constexpr
    decltype(auto) get(const wref_tuple<Tuples...>& t) noexcept
    {
        using dtl::wref_tuple::find_leaf;
        using dtl::wref_tuple::leaf;
        return [] <::std::size_t Offset, typename Reference> (
            const leaf<Offset, Reference>& part)
            -> decltype(auto)
        {
            return get<I - Offset>(::std::forward<Reference>(part.tuple));
        }((find_leaf<I>)(t));
    }
    
}

namespace std
{
    template<typename... Tuples>
    struct tuple_size<::yymp::wref_tuple<Tuples...>>
        : ::std::integral_constant<
            ::std::size_t,
            (::std::size_t{0} +
                ... + tuple_size<::std::remove_cvref_t<Tuples>>::value)
        > { };
    
    template<std::size_t I, typename... Tuples>
        requires (I < tuple_size<::yymp::wref_tuple<Tuples...>>::value)
    struct tuple_element<I, ::yymp::wref_tuple<Tuples...>>
    {
        using type = typename decltype(
            ::yymp::dtl::wref_tuple::find_element_type_marker<I>(
                ::std::declval<::yymp::wref_tuple<Tuples...>>())
        )::type;
    };
}

#endif // YYMP_WREF_TUPLE_HPP