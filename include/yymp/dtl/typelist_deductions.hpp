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
    
    template<::std::size_t MajorIndex> struct empty_deduction_list { };
    
    template<::std::size_t Offset, typename... Types, ::std::size_t... Is>
    struct deduction_list<
        Offset, ::yymp::typelist<Types...>, ::std::index_sequence<Is...>
    > : deduction_leaf<Offset + Is, Types>... { };
    
    template<std::size_t MajorIndex, ::std::size_t Offset, class TypeList>
    using make_deduction_list = ::std::conditional_t<
        TypeList::size != 0,
        deduction_list<Offset, TypeList>,
        empty_deduction_list<MajorIndex>
    >;
    
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
    > : make_deduction_list<Is, offsets_for<TypeLists::size...>[Is], TypeLists>...
    { 
        static inline constexpr ::std::size_t total_size
            = (::std::size_t{0} + ... + TypeLists::size);
    };
    
    template<
        class TypeLists,
        class TotalIndexSequence = ::std::make_index_sequence<
            wide_deduction_list<TypeLists>::total_size
        >
    > struct join_impl;
    
    template<class... TypeLists, ::std::size_t... I>
    struct join_impl<
        ::yymp::typelist<TypeLists...>, 
        ::std::index_sequence<I...>
    > {
        static constexpr wide_deduction_list<typelist<TypeLists...>> wdl = {};
        
        using type = ::yymp::typelist<decltype(
            ::yymp::dtl::typelist_deductions::deduce_type<I>(wdl)
        )...>;
    };
}

#endif // YYMP_DTL_TYPELIST_DEDUCTIONS_HPP