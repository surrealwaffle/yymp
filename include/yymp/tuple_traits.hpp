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
    
    // =========================================================================
    // The following specializations provide massive improvements in compile 
    // times when used in certain pathological cases.
    // These specializations drastically reduce the quadratic term involved in 
    // many chained stuple_cats.
    
    template<typename... Types>
    struct is_piecewise_reinitializable<const stuple<Types...>&>
        : ::std::is_copy_constructible<stuple<Types...>>::type { };
    
    template<typename... Types>
    struct is_piecewise_reinitializable<stuple<Types...>&&>
        : ::std::is_move_constructible<stuple<Types...>>::type { };
    
    template<typename... Types>
    struct is_piecewise_reinitializable<stuple<Types...>>
        : ::std::is_move_constructible<stuple<Types...>>::type { };
        
    template<typename... Types>
    struct is_piecewise_nothrow_reinitializable<const stuple<Types...>&>
        : ::std::is_nothrow_copy_constructible<stuple<Types...>>::type { };
    
    template<typename... Types>
    struct is_piecewise_nothrow_reinitializable<stuple<Types...>&&>
        : ::std::is_nothrow_move_constructible<stuple<Types...>>::type { };
    
    template<typename... Types>
    struct is_piecewise_nothrow_reinitializable<stuple<Types...>>
        : ::std::is_nothrow_move_constructible<stuple<Types...>>::type { };
}

#endif // YYMP_TUPLE_TRAITS_HPP