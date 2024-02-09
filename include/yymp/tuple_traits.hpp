// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_TUPLE_TRAITS_HPP
#define YYMP_TUPLE_TRAITS_HPP

#include <type_traits>
#include <utility>

#include <yymp/dtl/piecewise_reinitializable.hpp>

namespace yymp
{
    /** 
     * \brief Determines if all the element types of a \a Tuple have a 
     *        constructor that accepts the elements extracted from a \a Tuple,
     *        as if synthesized by `std::declval<Tuple>()`.
     */ 
    template<typename Tuple>
    struct is_piecewise_reinitializable
        : ::yymp::dtl::tuple_traits::is_piecewise_reinitializable<
            Tuple
        > { };
    
    /**
     * \brief Determines if all the element types of a \a Tuple have a 
     *        constructor that accepts the elements extracted from a \a Tuple,
     *        as if synthesized by `std::declval<Tuple>()`, which is known to 
     *        not throw any exceptions.
     */ 
    template<typename Tuple>
    struct is_piecewise_nothrow_reinitializable
        : ::yymp::dtl::tuple_traits::is_piecewise_nothrow_reinitializable<
            Tuple
        > { };
    
    /** 
     * \brief Constraint for #is_piecewise_reinitializable.
     */
    template<typename Tuple>
    concept piecewise_reinitializable
        = is_piecewise_reinitializable<Tuple>::value;
    
    /** 
     * \brief Constraint for #is_piecewise_nothrow_reinitializable.
     */
    template<typename Tuple>
    concept piecewise_nothrow_reinitializable
        = is_piecewise_nothrow_reinitializable<Tuple>::value;
}

#endif // YYMP_TUPLE_TRAITS_HPP