//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <yymp/typelist_fwd.hpp>
#include <yymp/setops.hpp>

#include <type_traits>

using ::yymp::typelist;
using ::yymp::intersection;
using ::yymp::difference;

using ::std::is_same;

struct a;
struct b;
struct c;
struct d;

using empty_typelist = typelist<>;
using all = typelist<a, b, c, d>;

/* yymp::intersection test cases:
 * (1) Intersecting with empty typelist yields the empty typelist, on both sides
 * (2) Intersecting disjoint typelists yields the empty typelist
 * (3) Intersecting typelists with common types preserves order in the first parameter
 * (4) intersection preserves duplicates in the first parameter
 * (5) intersection preserves duplicates ONLY in the first parameter
 */

static_assert(is_same<empty_typelist, typename intersection<empty_typelist, typelist<a,b,c,d>>::type>(), "(1) intersection on left with empty typelist must be the empty typelist");
static_assert(is_same<empty_typelist, typename intersection<typelist<a,b,c,d>,empty_typelist>::type>(), "(1) intersection on right with empty typelist must be the empty typelist");

static_assert(is_same<empty_typelist, typename intersection<typelist<a,b>, typelist<c,d>>::type>(), "(2) intersection of disjoint typelists must be the empty typelist");

static_assert(is_same<typelist<b,c>, typename intersection<typelist<a,b,c>, typelist<d,c,b>>::type>(), "(3) intersection of typelists preserves order in the first typelist");

static_assert(is_same<typelist<b, c, b>, typename intersection<typelist<a, b, c, b>, typelist<b, c>>::type>(), "(4) intersection preserves duplicates in the first parameter");

static_assert(is_same<typelist<b, c>, typename intersection<typelist<a, b, c>, typelist<b, b, c>>::type>(), "(5) intersection preserves duplicates ONLY in the first parameter");
static_assert(is_same<typelist<b, c, b>, typename intersection<typelist<a, b, c, b>, typelist<b, b, c>>::type>(), "(5) intersection preserves duplicates ONLY in the first parameter");

/* yymp::difference test cases:
 * (1) {}\all is {}
 * (2) all\{} is all
 * (3) difference preserves the order in the first parameter
 * (4) difference preserves duplicates in the first parameter
 * (5) difference does not preserve duplicates in the second parameter
 */
 
static_assert(is_same<empty_typelist, typename difference<empty_typelist, all>::type>(), "(1) difference with empty typelist as first parameter must be the empty typelist");

static_assert(is_same<all, typename difference<all, empty_typelist>::type>(), "(2) difference with empty typelist as the second parameter must be the first parameter");

static_assert(is_same<typelist<a, c>, typename difference<all, typelist<b, d>>::type>(), "(3) difference must preserve order in the first parameter");

static_assert(is_same<typelist<a, b, a>, typename difference<typelist<a, b, c, a, d>, typelist<c, d>>::type>(), "(4) difference must preserve duplicates in the first parameter");

static_assert(is_same<typelist<a, c>, typename difference<all, typelist<b, b, d>>::type>(), "(5) difference must not preserve duplicates in the second parameter");