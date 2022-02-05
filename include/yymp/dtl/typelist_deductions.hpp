// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_DTL_TYPELIST_DEDUCTIONS_HPP
#define YYMP_DTL_TYPELIST_DEDUCTIONS_HPP

#include <cstddef>

#include <array>
#include <utility>

namespace yymp
{
    template<typename... Types>
    struct typelist;
    
    template<typename T>
    struct is_typelist;
}

namespace yymp::dtl::typelist_deductions
{
    template<auto... Sizes>
    inline constexpr auto offsets_for = [] () constexpr {
        ::std::array<::std::size_t, sizeof...(Sizes)> offsets {Sizes...};
        
        ::std::size_t offset = 0;
        for (auto& x : offsets)
        {
            const ::std::size_t size = x;
            x = offset;
            offset += size;
        }
        
        return offsets;
    }();
    
    template<::std::size_t I, typename Type>
    struct deduction_leaf { };
    
    template<
        ::std::size_t Offset, 
        class TypeList, 
        class IndexSequence = ::std::make_index_sequence<TypeList::size>
    > struct deduction_list;
    
    template<::std::size_t Offset, typename... Types, ::std::size_t... Is>
    struct deduction_list<
        Offset, ::yymp::typelist<Types...>, ::std::index_sequence<Is...>
    > : deduction_leaf<Offset + Is, Types>... { };
    
        
    template<::std::size_t I, typename Type>
    Type
    deduce_type(const deduction_leaf<I, Type>&) noexcept; // no definition
    
    template<typename Type, ::std::size_t I>
    ::std::integral_constant<::std::size_t, I> 
    deduce_index(const deduction_leaf<I, Type>&) noexcept; // no definition
    
    template<
        class TypeLists,
        class IndexSequence = ::std::make_index_sequence<TypeLists::size>
    > struct wide_deduction_list;
    
    template<typename... TypeLists, ::std::size_t... Is>
    struct wide_deduction_list<
        ::yymp::typelist<TypeLists...>,
        ::std::index_sequence<Is...>
    >
        : deduction_list<offsets_for<TypeLists::size>[Is], TypeLists>...
    { 
        static inline constexpr ::std::size_t total_size
            = (::std::size_t{0} + ... + TypeLists::size);
        
        using as_typelist = decltype(
            [] <::std::size_t... Js> 
            (::std::index_sequence<Js...>) constexpr
            {
                return ::yymp::typelist<decltype(
                    (deduce_type<Js>)(::std::declval<wide_deduction_list>())
                )...>{};
            }(::std::make_index_sequence<total_size>{})
        );

    };
}

#endif // YYMP_DTL_TYPELIST_DEDUCTIONS_HPP