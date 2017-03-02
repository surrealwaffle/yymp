//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef YYMP__TYPELIST_FWD_HPP
#define YYMP__TYPELIST_FWD_HPP

namespace yymp {

/**
 * \brief Represents a compile-time sequence of types.
 *
 * \tparam Ts The parameter pack representing the sequence.
 */
template< class... Ts >
struct typelist {
    using type = typelist;
};

}

#endif // YYMP__TYPELIST_FWD_HPP
