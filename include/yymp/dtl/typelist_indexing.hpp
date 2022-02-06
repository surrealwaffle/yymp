// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_DTL_TYPELIST_INDEXING_HPP
#define YYMP_DTL_TYPELIST_INDEXING_HPP

#include <cstddef>

#include <utility>
#include <type_traits>

namespace yymp
{
    template<typename... Types>
    struct typelist;
    
    template<class T>
    struct is_typelist;
    
    template<class... TypeLists>
        requires (is_typelist<TypeLists>::value && ...)
    struct typelist_join;
    
    template<template<typename...> class TransformTrait, class TypeList>
    struct typelist_transform;
    
    template<template<typename...> class TransformationTrait, class TypeList>
    struct typelist_expand_trait;
}

namespace yymp::dtl::typelist_indexing
{
    template<::std::size_t I, typename T>
    struct indexed_type_marker
    {
        static constexpr auto index = I;
        
        using type = T;
    };
    
    template<
        class TypeList, 
        class IndexSequence = ::std::make_index_sequence<TypeList::size>
    > struct mark_typelist;
    
    template<typename... Types, ::std::size_t... Is>
    struct mark_typelist<typelist<Types...>, ::std::index_sequence<Is...>>
    {
        using type = typelist<indexed_type_marker<Is, Types>...>;
    };
    
    template<class MarkedTypeList>
    struct marked_typelist_to_index_sequence;
    
    template<::std::size_t... Is, typename... Types>
    struct marked_typelist_to_index_sequence<
        typelist<indexed_type_marker<Is, Types>...>
    > { using type = ::std::index_sequence<Is...>; };
    
    template<typename T, class TypeList>
    struct indices_of
    {
        using marked_typelist = typename mark_typelist<TypeList>::type;
        
        template<typename U>
        using transform = ::std::conditional<
            ::std::is_same<T, typename U::type>::value,
            typelist<U>,
            typelist<>
        >;
        
        using type = typename marked_typelist_to_index_sequence<
            typename typelist_expand_trait<
                typelist_join,
                typename typelist_transform<
                    transform,
                    marked_typelist
                >::type
            >::type
        >::type;
    };
}

#endif // YYMP_DTL_TYPELIST_INDEXING_HPP