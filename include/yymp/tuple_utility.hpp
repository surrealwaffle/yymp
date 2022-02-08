// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_TUPLE_UTILITY_HPP
#define YYMP_TUPLE_UTILITY_HPP

#include <cstddef>

#include <functional>
#include <utility>
#include <type_traits>

namespace yymp
{
    /**
     * \brief Alias for an `std::index_sequence` for the indices of \a Tuple.
     */
    template<typename Tuple>
    using make_tuple_index_sequence = ::std::make_index_sequence<
        ::std::tuple_size<::std::remove_cvref_t<Tuple>>::value
    >;
    
    /**
     * \brief Mirrors `std::apply`, but uses `get` found through ADL rather than
     *        `std::get` to extract elements from the \a tuple.
     */
    template<class F, class Tuple>
    constexpr
    decltype(auto) apply(F&& f, Tuple&& t)
    {
        using value_type = ::std::remove_cvref_t<Tuple>;
        return [&f, &t] <::std::size_t... I> (::std::index_sequence<I...>)
            -> decltype(auto)
        {
            return ::std::invoke(
                ::std::forward<F>(f),
                get<I>(::std::forward<Tuple>(t))...
            );
        }(::std::make_index_sequence<::std::tuple_size<value_type>::value>{});
    }
}

#endif // YYMP_TUPLE_UTILITY_HPP