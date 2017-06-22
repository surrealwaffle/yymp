//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef YYMP__FOR_EACH_HPP
#define YYMP__FOR_EACH_HPP

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
constexpr UnaryFunction for_each(typelist<Ts...> types, UnaryFunction f) {
    (void)types;
    return ((..., (f(type_value<T>{}), void())) , std::move(f));
}

#else

template< class... Ts, class UnaryFunction >
constexpr UnaryFunction for_each(typelist<Ts...> types, UnaryFunction f) {
    (void)types;
    using discard = int[sizeof...(Ts)];
    (void)discard{(f(type_value<Ts>{}), void(), 0)..., 0};
    return std::move(f);
}

#endif // __cpp_fold_expressions >= 201411

}

#endif // YYMP__FOR_EACH_HPP