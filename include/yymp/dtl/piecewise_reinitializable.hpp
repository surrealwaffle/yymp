// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_DTL_TUPLE_REINITIALIZABLE_HPP
#define YYMP_DTL_TUPLE_REINITIALIZABLE_HPP

#include <utility>
#include <type_traits>

namespace yymp::dtl::tuple_traits
{
    template<
        typename Tuple, 
        class = ::std::make_index_sequence<
            ::std::tuple_size<::std::remove_cvref_t<Tuple>>::value
        >
    > struct is_piecewise_reinitializable;
    
    template<
        typename Tuple, 
        class = ::std::make_index_sequence<
            ::std::tuple_size<::std::remove_cvref_t<Tuple>>::value
        >
    > struct is_piecewise_nothrow_reinitializable;
    
    template<typename Tuple, ::std::size_t... I>
    struct is_piecewise_reinitializable<Tuple, ::std::index_sequence<I...>>
        : ::std::bool_constant<(
            ::std::is_constructible_v<
                typename ::std::tuple_element<I, ::std::remove_cvref_t<Tuple>>::type,
                decltype(get<I>(::std::declval<Tuple>()))
            > && ...
        )> { };
    

    
    template<typename Tuple, ::std::size_t... I>
    struct is_piecewise_nothrow_reinitializable<Tuple, ::std::index_sequence<I...>>
        : ::std::bool_constant<(
            ::std::is_nothrow_constructible_v<
                typename ::std::tuple_element<I, ::std::remove_cvref_t<Tuple>>::type,
                decltype(get<I>(::std::declval<Tuple>()))
            > && ...
        )> { };
        
}

#endif // YYMP_DTL_TUPLE_REINITIALIZABLE_HPP