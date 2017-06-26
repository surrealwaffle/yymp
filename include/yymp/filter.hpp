//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef YYMP__FILTER_HPP
#define YYMP__FILTER_HPP

#include <yymp/typelist_fwd.hpp>

#include <yymp/transform.hpp>
#include <yymp/group.hpp>

#include <type_traits> // std::conditional

namespace yymp {

template< template< class... > class Predicate, class TypeList >
struct filter;

template< class T, class TypeList >
struct filter_of;

template< class TypeList >
struct filter_duplicates;

/** \brief Retains all types `T` in \a Ts for which <code>Predicate<T>::value</code> evaluates to \c true in-order as \ref yymp::typelist "typelist" `type`.
 *
 * Duplicates are retained.
 *
 * \tparam Predicate The template class predicate that passes types that meet the criteria.
 * \tparam Ts The sequence of types to filter.
 */
template< template< class... > class Predicate, class... Ts >
struct filter< Predicate, typelist< Ts... > > {
    template< class T >
    struct TPredicate {
        using type = typename std::conditional< Predicate< T >::value, std::true_type, std::false_type >::type;
    };

    /** \brief The \ref yymp::typelist "typelist" containing the types in \a Ts that pass \a Predicate, in the order they occur in \a Ts.
     *
     * Duplicates are retained.
     */
    using type = typename get_group<
        std::true_type,
        typename group_by<
            TPredicate,
            typelist< Ts... >
        >::type
    >::type;
};

/** \brief Retains all occurrences of \a T in the sequence \a Ts, as \ref yymp::typelist "typelist" `type`.
 *
 * Duplicates are retained.
 *
 * \tparam T The type to retain.
 * \tparam Ts The sequence of types to filter.
 */
template< class T, class... Ts >
struct filter_of< T, typelist< Ts... > > {
    template< class U >
    struct identity { using type = U; };

    /** \brief The \ref yymp::typelist "typelist" containing all occurrences of \a T in \a Ts.
     *
     * Duplicates are retained.
     */
    using type = typename get_group<
        T,
        typename group_by<
            identity,
            typelist< Ts... >
        >::type
    >::type;
};

/** \brief Retains only the first occurrence of each type in \a Ts as \ref yymp::typelist "typelist" `type`.
 *
 * \tparam Ts The type sequence to filter for duplicates.
 */
template< class... Ts >
struct filter_duplicates< typelist< Ts... > > {
    template< class T >
    struct identity { using type = T; };

    /** \brief The \ref yymp::typelist "typelist" containing the distinct types of \a Ts, in the order they occur. */
    using type = typename transform<
        get_group_key,
        typename group_by< identity, typelist< Ts... > >::type
    >::type;
};

}

#endif // YYMP__FILTER_HPP
