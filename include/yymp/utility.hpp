// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_UTILITY_HPP
#define YYMP_UTILITY_HPP

#include <cstddef>

#include <type_traits>
#include <utility>

namespace yymp
{
    template<typename T> struct type_marker { using type = T; };
    
    template<auto IndexArray, ::std::size_t Size = IndexArray.size()>
    using array_to_index_sequence = decltype(
        [] <std::size_t... I> (::std::index_sequence<I...>) constexpr
        {
            return ::std::index_sequence<IndexArray[I]...>{};
        }(::std::make_index_sequence<Size>{})
    );
}

#endif // YYMP_UTILITY_HPP