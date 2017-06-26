//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/* Test Cases:
 * join
 *   (1) empty typelist is identity of join, left and right
 *   (2) join of nothing is the empty typelist
 *   (3) join of single typelist is identity operation
 *   (4) join of non-typelists can be taken but not necessarily defined
 *   (5) join of multiple typelists
 */

#include <yymp/typelist_fwd.hpp>
#include <yymp/join.hpp>

#include <type_traits>

using ::yymp::typelist;
using ::yymp::join;

struct fwd;
struct fwd2;
struct fwd3;

using empty_typelist = typelist<>;

////////////////
// (1) empty typelist is identity of join, on left and right
static_assert(std::is_same<empty_typelist, typename join<empty_typelist, empty_typelist>::type>(), "empty typelist must be identity of join operation");
static_assert(std::is_same<typelist<fwd>, typename join<empty_typelist, typelist<fwd>>::type>(), "empty typelist must be identity of join operation (on left)");
static_assert(std::is_same<typelist<fwd>, typename join<typelist<fwd>, empty_typelist>::type>(), "empty typelist must be identity of join operation (on right)");

////////////////
// (2) join of nothing is the empty typelist
static_assert(std::is_same<empty_typelist, typename join<>::type>(), "join of nothing is the empty typelist");

////////////////
// (3) join of single typelist is identity operation
static_assert(std::is_same<typelist<fwd>, typename join<typelist<fwd>>::type>(), "join of single typelist must be identity operation");
static_assert(std::is_same<typelist<fwd, fwd2>, typename join<typelist<fwd, fwd2>>::type>(), "join of single typelist must be identity operation");

////////////////
// (4) join of non-typelists can be taken but not necessarily defined
join<fwd>* a;
join<fwd, fwd2>* b;

////////////////
// (5) join of multiple typelists
static_assert(std::is_same<typelist<fwd, fwd2, fwd3>, typename join<typelist<fwd>, typelist<fwd2>, typelist<fwd3>>::type>(), "join can be taken over several typelists");
