// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_BYTE_ENABLE_HPP
#define YYMP_BYTE_ENABLE_HPP

#include <cstddef>

#include <type_traits>

// =============================================================================
// =============================================================================
// SYNOPSIS
// This file provides the enable_byte_trait class template, which is specialized on
// types which serve a byte-like purpose for facilities provided in yymp.
//
// Specializations enable_byte_trait<unsigned char> and enable_byte_trait<std::byte>
// are provided below.

namespace yymp
{
   /**
    * \brief Indicates if \a T should be enabled as a byte-like type for relevant
    *        facilities.
    *
    * Users may enable these facilities for their own types by specializing 
    * `enable_byte_trait` on their type with an empty body.
    *
    * The types byte-enabled through this trait should be explicitly convertible
    * to and from `std::byte` and `unsigned char` with no loss of value.
    *
    * \sa byte_enabled
    */
   template<typename T>
   struct enable_byte_trait { using disabled = void; };
   
   /**
    * \brief A constraint which admits \a T if it is byte-enabled through 
    *        `enable_byte_trait`.
    *
    * \sa enable_byte_trait
    */
   template<typename T>
   concept byte_enabled = !(requires { typename enable_byte_trait<T>::disabled; });
   
   template<> struct enable_byte_trait<unsigned char> {};
   template<> struct enable_byte_trait<std::byte> {};
   
   template<typename T>
   struct enable_byte_trait<const T> : enable_byte_trait<T> {};
   
   template<typename T>
   struct enable_byte_trait<volatile T> : enable_byte_trait<T> {};
   
   template<typename T>
   struct enable_byte_trait<const volatile T> : enable_byte_trait<T> {};
}

#endif // YYMP_BYTE_ENABLE_HPP