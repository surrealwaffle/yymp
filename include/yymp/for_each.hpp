//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef YYMP__FOR_EACH_HPP
#define YYMP__FOR_EACH_HPP

#include <cstddef>

#include <utility>

#include <yymp/typelist_fwd.hpp>

namespace yymp {

/** \brief A value that represents a type.
 *
 * \tparam T The type to represent.
 */
template< class T >
struct type_value {
    /** \brief The represented type. */
    using type = T;
};

/** \brief Calls \a f on `type_value<T>{}` for each type `T` in \a types, in order.
 *
 * \return `std::move(f)`
 */
template< class... Ts, class UnaryFunction >
constexpr UnaryFunction for_each(typelist<Ts...> types, UnaryFunction f);

#if __cpp_fold_expressions >= 201411

template< class... Ts, class UnaryFunction >
constexpr UnaryFunction for_each(typelist<Ts...>, UnaryFunction f) {
    return ((..., (f(type_value<Ts>{}), void())) , std::move(f));
}

#elif __cpp_constexpr >= 201304

template< class... Ts, class UnaryFunction >
constexpr UnaryFunction for_each(typelist<Ts...>, UnaryFunction f) {
    using discard = int[sizeof...(Ts) + 1];
    (void)discard{(f(type_value<Ts>{}), void(), 0)...};
    return std::move(f);
}

#else

namespace detail {
    template< std::size_t N, class UnaryFunction >
    constexpr UnaryFunction discard(int const (&)[N], UnaryFunction f) {
        return std::move(f);
    }
}

// requires a bit of a hack to make this work in C++11
template< class... Ts, class UnaryFunction >
constexpr UnaryFunction for_each(typelist<Ts...>, UnaryFunction f) {
    using discard = int[sizeof...(Ts) + 1];
    return ::yymp::detail::discard(discard{(f(type_value<Ts>{}), void(), 0)...}, std::move(f));
}

#endif

}

#endif // YYMP__FOR_EACH_HPP
