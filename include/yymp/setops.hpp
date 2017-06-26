//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef YYMP__SETOPS_HPP
#define YYMP__SETOPS_HPP

#include <yymp/typelist_fwd.hpp>

#include <yymp/filter.hpp>
#include <yymp/traits.hpp>
#include <yymp/join.hpp>

namespace yymp {

/////////////////////////////////////
// DECLARATIONS
//

/** \brief Preserves the types that are in both \a A and \a B as \ref yymp::typelist "typelist" `type`.
 * 
 * The preserved types are in the order of \a A.
 * \tparam A A \ref yymp::typelist "typelist".
 * \tparam B A \ref yymp::typelist "typelist".
 */
template< class A, class B >
struct intersection;

/** \brief Preserves the types in \a A that are not in \a B as \ref yymp::typelist "typelist" `type`.
 *
 * The preserved types are in the order of \a A.
 * \tparam A A \ref yymp::typelist "typelist".
 * \tparam B A \ref yymp::typelist "typelist".
 */
template< class A, class B >
struct difference;

/** \brief Preserves the types of \a A and \a B that do not belong to both \a A and \a B.
 *
 * The preserved types are in the order of \a A first and then \a B.
 * \tparam A A \ref yymp::typelist "typelist".
 * \tparam B A \ref yymp::typelist "typelist".
 */
template< class A, class B >
struct symmetric_difference;

/////////////////////////////////////
// IMPLEMENTATION
//

template< class... As, class... Bs >
struct intersection< typelist<As...>, typelist<Bs...> > {
    using A = typelist<As...>;
    using B = typelist<Bs...>;
    
    template< class T >
    using in_B = any_of<T, B>;
    
    using type = typename filter<in_B, A>::type; 
};

template< class... As, class... Bs >
struct difference< typelist<As...>, typelist<Bs...> > {
    using A = typelist<As...>;
    using B = typelist<Bs...>;
    
    template< class T >
    using not_in_B = none_of<T, B>;
    
    using type = typename filter<not_in_B, A>::type;
};

template< class... As, class... Bs >
struct symmetric_difference< typelist<As...>, typelist<Bs...> > {
    using A = typelist<As...>;
    using B = typelist<Bs...>;
    
    template< class T >
    using not_in_A = none_of<T, A>;
    
    template< class T >
    using not_in_B = none_of<T, B>;
    
    using type = typename yymp::join<
        typename filter<not_in_B, A>::type,
        typename filter<not_in_A, B>::type
    >;
};

}

#endif // YYMP__SETOPS_HPP