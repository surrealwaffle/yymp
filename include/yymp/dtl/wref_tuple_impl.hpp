// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_DTL_WREF_TUPLE_IMPL_HPP
#define YYMP_DTL_WREF_TUPLE_IMPL_HPP

#include <cstddef>

#include <algorithm>
#include <array>
#include <concepts>
#include <functional>
#include <numeric>
#include <type_traits>
#include <utility>

#include <yymp/utility.hpp>

namespace yymp::dtl::wref_tuple
{
    template<class... Tuples>
    constexpr std::array wide_offsets = [] () constexpr
    {
        ::std::array<::std::size_t, sizeof...(Tuples)> result = {
            ::std::tuple_size<::std::remove_cvref_t<Tuples>>::value...
        };
        
        if constexpr (sizeof...(Tuples) != 0)
        {
            ::std::shift_right(result.begin(), result.end(), 1);
            result[0] = 0;
        }
        
        ::std::partial_sum(result.cbegin(), result.cend(), result.begin());
        
        return result;
    }();
    
    template<std::size_t I, class... Tuples>
      requires (I < (::std::tuple_size<::std::remove_cvref_t<Tuples>>::value + ... + std::size_t(0)))
    constexpr std::size_t leaf_index_of = [] () constexpr {
      constexpr auto& Biases = wide_offsets<Tuples...>;
      
      constexpr auto it = ::std::upper_bound(
          Biases.cbegin(), Biases.cend(),
          I);
      static_assert(it != Biases.cbegin());
      return *(it - 1);
    }();
    
    /**
     * \brief The leaf type of a wide reference tuple implementation.
     * 
     * \sa find_leaf
     */
    template<::std::size_t Offset, typename Tuple>
    struct leaf { Tuple&& tuple; };
    
    template<::std::size_t IMajor, typename Tuple>
    struct empty_leaf { Tuple&& tuple; };
    
    template<::std::size_t IMajor, ::std::size_t Offset, typename Tuple>
    using make_leaf = ::std::conditional_t<
        (::std::tuple_size<::std::remove_cvref_t<Tuple>>::value > 0),
        leaf<Offset, Tuple>,
        empty_leaf<IMajor, Tuple>
    >;
    
    template<class IndexSequence, typename... Tuples>
    struct impl;
    
    /**
     * \brief The implementation of a wide reference tuple.
     */
    template<::std::size_t... IMajor, typename... Tuples>
    struct impl<::std::index_sequence<IMajor...>, Tuples...>
        : make_leaf<IMajor, wide_offsets<Tuples...>[IMajor], Tuples>... { };
    
    template<typename... Tuples>
    using make_impl = impl<
        ::std::make_index_sequence<sizeof...(Tuples)>,
        Tuples...
    >;
    
    /**
     * \brief Locates the #leaf from which the wide-tuple element by index \a I
     *        can be extracted.
     *
     * \return A constant reference to the #leaf.
     */
    template<
        ::std::size_t I,
        class IndexSequence, typename... Tuples
    > 
    constexpr
    const auto& find_leaf(const impl<IndexSequence, Tuples...>& t)
        noexcept
    {
        // deduce the leaf
        return [] <typename Tuple> (const leaf<leaf_index_of<I, Tuples...>, Tuple>& part) 
            -> const auto&
        {
            return part;
        }(t);
    }
    
    template<
        ::std::size_t I,
        class IndexSequence, typename... Tuples
    > constexpr
    auto find_element_type_marker(
        const impl<IndexSequence, Tuples...>& t)
        noexcept
    {
        return [] <::std::size_t Offset, typename Tuple> (
            const leaf<Offset, Tuple>&)
        {
            using tuple_type = ::std::remove_cvref_t<Tuple>;
            return type_marker<
                typename ::std::tuple_element<I - Offset, tuple_type>::type
            >{};
        }((find_leaf<I>)(t));
    }
}

#endif // YYMP_DTL_WREF_TUPLE_IMPL_HPP