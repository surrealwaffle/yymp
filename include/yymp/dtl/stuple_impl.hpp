// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_DTL_STUPLE_IMPL_HPP
#define YYMP_DTL_STUPLE_IMPL_HPP

#include <functional>
#include <type_traits>
#include <utility>

namespace yymp
{
    template<typename... Types>
    struct stuple; // forward declare for later
}

namespace yymp::dtl::stuple
{
    template<typename T>
    struct is_stuple : std::false_type { };
    
    template<typename... Types>
    struct is_stuple<::yymp::stuple<Types...>> : std::true_type { };
    
    // =========================================================================
    
    /**
     * \brief Implements the leaf of a `stuple` implementation.
     */
    template<::std::size_t Index, typename Type>
    struct leaf { Type part; };
    
    /**
     * \brief The `stuple` implementation.
     */
    template<class IndexSequence, typename... Types>
    struct impl;
    
    template<::std::size_t... I, typename... Types>
    struct impl<::std::index_sequence<I...>, Types...> : leaf<I, Types>... { };
    
    /**
     * \brief Convenience alias for creating `stuple` implementations.
     */
    template<typename... Types>
    using make_impl = impl<::std::make_index_sequence<sizeof...(Types)>, Types...>;
    
    template<
        class Tuple, 
        typename IndexSequence = ::std::make_index_sequence<
            ::std::tuple_size<Tuple>::value
        >
    > struct transform_to_stuple;
    
    template<class Tuple, ::std::size_t... I>
    struct transform_to_stuple<Tuple, ::std::index_sequence<I...>>
    {
        using type = ::yymp::stuple<
            typename ::std::tuple_element<
                I,
                Tuple
            >::type...
        >;
    };
    
    // =========================================================================
    // stuple::impl: deduction helpers
    
    template<::std::size_t I, typename T>
    T
    deduce_type(const leaf<I, T>&) noexcept;
    
    template<typename T, ::std::size_t I>
    ::std::integral_constant<::std::size_t, I> 
    deduce_index(const leaf<I, T>&) noexcept;
    
    // =========================================================================
    // stuple::impl: index-based get
    
    template<::std::size_t I, class T>
    constexpr
    T&
    get(leaf<I, T>& p) noexcept { return p.part; }
    
    template<::std::size_t I, class T>
    constexpr
    const T& 
    get(const leaf<I, T>& p) noexcept { return p.part; }
    
    template<::std::size_t I, class T>
    constexpr
    T&& 
    get(leaf<I, T>&& p) noexcept { return ::std::forward<T&&>(p.part); }
    
    template<::std::size_t I, class T>
    constexpr
    const T&& 
    get(const leaf<I, T>&& p) noexcept { return ::std::forward<const T&&>(p.part); }
    
    // =========================================================================
    // stuple::impl: type-based get
    
    template<class T, ::std::size_t I>
    constexpr
    T&
    get(leaf<I, T>& p) noexcept { return p.part; }
    
    template<class T, ::std::size_t I, ::std::size_t... Is, class... Types>
    constexpr
    const T& 
    get(const leaf<I, T>& p) noexcept { return p.part; }
    
    template<class T, ::std::size_t I>
    constexpr
    T&& 
    get(leaf<I, T>&& p) noexcept { return ::std::forward<T&&>(p.part); }
    
    template<class T, ::std::size_t I>
    constexpr
    const T&& 
    get(const leaf<I, T>&& p) noexcept { return ::std::forward<const T&&>(p.part); }
    
    // =========================================================================
    // stuple::impl: apply
    
    template<typename F, ::std::size_t... I, typename... T>
    constexpr decltype(auto) apply(
      F&& f, 
      const impl<::std::index_sequence<I...>, T...>& t) 
    {
      return ::std::invoke(::std::forward<F>(f), t.leaf<I, T>.part...);
    }
    
    template<typename F, ::std::size_t... I, typename... T>
    constexpr decltype(auto) apply(
      F&& f, 
      impl<::std::index_sequence<I...>, T...>&& t) 
    {
      return ::std::invoke(::std::forward<F>(f), ::std::move(t.leaf<I, T>.part)...);
    }
}

#endif // YYMP_DTL_STUPLE_IMPL_HPP