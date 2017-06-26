//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/* Test Cases:
 * (1) the true predicate yields the input
 * (2) the false predicate yields the empty typelist
 * (3) predicate<T> only requires constexpr static member value (bool)
 * (4) predicate<T> only requires constexpr static member value (implicitly convertible to bool)
 * (5) predicate<T...> is an acceptable predicate for filter
 * (5.1) variadic template alias is an acceptable predicate for filter
 * (6) use of std::* predicates
 */
 
#include <yymp/typelist_fwd.hpp>
#include <yymp/filter.hpp>

#include <type_traits>

using ::yymp::typelist;
using ::yymp::filter;

using ::std::is_same;

struct A;
struct B;
struct C;

using tlist = typelist<A, B, C>;

//////////////////////
// (1) true predicate yields input

template< class T >
struct true_predicate : std::true_type { };

static_assert(is_same<tlist, typename filter<true_predicate, tlist>::type>(), "true predicate must yield input");

//////////////////////
// (2) false predicate yields empty typelist

template< class T >
struct false_predicate : std::false_type { };

static_assert(is_same<typelist<>, typename filter<false_predicate, tlist>::type>(), "false predicate must yield empty typelist");

//////////////////////
// (3) predicate<T> only requires constexpr static member value (bool)

template< class >
struct basic_predicate { static constexpr bool value = true; };

static_assert(is_same<tlist, typename filter<basic_predicate, tlist>::type>(), "basic predicate must yield input");

//////////////////////
// (4) predicate<T> only requires constexpr static member value (implicitly convertible to bool)

struct my_true_value {
    constexpr operator bool() const noexcept { return true; }
};

template< class >
struct basic_predicate2 { static constexpr my_true_value value = {}; };

static_assert(is_same<tlist, typename filter<basic_predicate2, tlist>::type>(), "basic predicate2 must yield input");

//////////////////////
// (5) predicate<T...> is an acceptable predicate for filter
// (5.1) variadic template alias is an acceptable predicate for filter

template< class... >
struct true_variadic_predicate : std::true_type { };

static_assert(is_same<tlist, typename filter<true_variadic_predicate, tlist>::type>(), "true variadic predicate must yield input");

//////////////////////
// (6) use of std::* predicates

using mylist = typelist<
    int,
    char,
    C*,
    int const*,
    bool,
    float,
    A,
    void,
    void*,
    C*
>;

static_assert(is_same<typelist<C*, int const*, void*, C*>, typename filter<std::is_pointer, mylist>::type>(), "");
static_assert(is_same<typelist<int, char, bool>, typename filter<std::is_integral, mylist>::type>(), "");
static_assert(is_same<typelist<int, char, bool, float>, typename filter<std::is_arithmetic, mylist>::type>(), "");
static_assert(is_same<typelist<int, char, bool, float, void>, typename filter<std::is_fundamental, mylist>::type>(), "");