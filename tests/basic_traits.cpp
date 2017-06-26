//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/* Test cases:
 * is_typelist (section is_typelist_tests0)
 *   empty typelist:                        value is true
 *   single-type typelist:                  value is true
 *   multi-type typelist:                   value is true 
 *   fundamental types (char, int, void):   value is false
 *   forward-declared type:                 value is false
 *
 * size (section size_tests0)
 *   size of empty typelist:                    value is 0
 *   size of singleton list:                    value is 1
 *   size of typelist with n distinct types:    value is n
 *   size of typelsit with n of the same types: value is n
 *   size<non-typelist>:                        can be taken, but class undefined
 *
 * is_empty (section is_empty_tests0)
 *   empty typelist:            value is true
 *   singleton typelist:        value is false
 *   multiple-types typelist:   value is false
 *   non-typelist:              can be taken, but class undefined
 *
 * is_not_empty (section is_not_empty_tests0)
 *   empty typelist:            value is false
 *   singleton typelist:        value is true
 *   multiple-types typelist:   value is true
 *   non-typelist:              can be taken, but class undefined
 *
 */
 
#include <yymp/typelist_fwd.hpp>

/* ******************
 * is_typelist_tests0
 */

using ::yymp::typelist;
using ::yymp::is_typelist;

struct fwd_type;

static_assert(is_typelist<typelist<>>::value, "the empty typelist must be a typelist");
static_assert(is_typelist<typelist<int>>::value, "typelist<int> must be a typelist");
static_assert(is_typelist<typelist<fwd_type>>::value, "typelist<fwd_type> must be a typelist");
static_assert(is_typelist<typelist<int, fwd_type, void, void*, fwd_type>>::value, "typelist of multiple types must be a typelist");
static_assert(!is_typelist<char>::value && !is_typelist<void>::value && !is_typelist<fwd_type>::value, "non-typelists must not detect as typelists");

/* ***********
 * size_tests0
 */

using ::yymp::size;

static_assert(size<typelist<>>::value == 0, "the empty typelist must have a size of 0");
static_assert(size<typelist<fwd_type>>::value == 1, "a singleton typelist must have a size of 1");
static_assert(size<typelist<int, void, fwd_type>>::value == 3, "a typelist of n distinct types must have a size of n");
static_assert(size<typelist<fwd_type, fwd_type, fwd_type>>::value == 3, "a typelist of n of the same type must have a size of n");
size<fwd_type>* t1_0; // size<> of non-typelist can be taken, but not necessarily defined

/* ***************
 * is_empty_tests0
 */

using ::yymp::is_empty;

static_assert(is_empty<typelist<>>::value, "the empty typelist must be empty");
static_assert(!is_empty<typelist<fwd_type>>::value, "singleton typelist must not be considered empty");
static_assert(!is_empty<typelist<void>>::value, "singleton typelist must not be considered empty");
static_assert(!is_empty<typelist<fwd_type, fwd_type, int, void>>::value, "non-empty typelist must not be considered empty");
is_empty<fwd_type>* t2_0; // is_empty of non-typelist can be taken, but not necessarily defined

/* *******************
 * is_not_empty_tests0
 */

using ::yymp::is_not_empty;

static_assert(!is_not_empty<typelist<>>::value, "the empty typelist must be empty");
static_assert(is_not_empty<typelist<fwd_type>>::value, "singleton typelist must not be considered empty");
static_assert(is_not_empty<typelist<void>>::value, "singleton typelist must not be considered empty");
static_assert(is_not_empty<typelist<fwd_type, fwd_type, int, void>>::value, "non-empty typelist must not be considered empty");
is_not_empty<fwd_type>* t3_0; // is_not_empty of non-typelist can be taken, but not necessarily defined
    
