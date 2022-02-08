// SPDX-License-Identifier: BSL-1.0

/*
 This source file is used to stress yymp::stuple against std::tuple.
 
 This is not a criticism of the std::tuple implementation; std::tuple has 
 particular requirements that can cause issues with compile times when overused.
 
 The stress test consists of accumulating into a tuple by overloading operator+ 
 via a wrapping type. In such a situation, one single concatenation operation 
 is preferred, but in some circumstances (e.g. in embedded DSLs) this is not 
 always doable.
 
 Usage:
    gcc -c stress.cpp -std=c++20 -I{PATH TO YYMP INCLUDE} -DTEST_DEPTH=50 -DYYMP_STRESS_STD_TUPLE
    gcc -c stress.cpp -std=c++20 -I{PATH TO YYMP INCLUDE} -DTEST_DEPTH=50
 
 The first line tests std::tuple. The second tests yymp::stuple.
 
 Total compile time (in seconds) on my system (using -ftime-report)
                TEST_DEPTH=50   TEST_DEPTH=100  TEST_DEPTH=150
 std::tuple     4.85            32.05           319.47
 yymp::stuple   1.01            2.62            5.95
 
 $ gcc --version
 gcc.exe (Rev8, Built by MSYS2 project) 11.2.0
 */

#include <cstddef>

#include <utility>
#include <type_traits>

// Set the default test depth, if not supplied at the command line
#ifndef TEST_DEPTH
#   define TEST_DEPTH 100
#endif

// Imitates some type that is distinct under some non-type template parameter.
template<auto> struct Unique { };

// Wraps a tuple type and overloads operator+ which concatenates to the tuple 
// on the right. 
// If TEST_STUPLE is defined, then yymp::stuple_cat is used.
// Otherwise, std::tuple_cat is used.
template<typename Tuple>
struct tuple_catter
{
    Tuple t;
}; template<typename Tuple> tuple_catter(Tuple) -> tuple_catter<Tuple>;

// Provides member type which is the tuple type that results from the test.
template<typename IndexSequence>
struct make_test_tuple;

#ifndef YYMP_STRESS_STD_TUPLE
#include <yymp/stuple.hpp>

template<typename TupleA, typename TupleB>
constexpr auto operator+(const tuple_catter<TupleA>& a, const tuple_catter<TupleB>& b)
{
    return tuple_catter{::yymp::stuple_cat(a.t, b.t)};
}

template<typename TupleA, typename X>
constexpr auto operator+(const tuple_catter<TupleA>& a, const X& x)
{
    return tuple_catter{::yymp::stuple_cat(a.t, ::yymp::stuple{x})};
}

constexpr ::yymp::stuple empty_tuple {};

template<::std::size_t... I>
struct make_test_tuple<::std::index_sequence<I...>>
{
    using type = ::yymp::stuple<Unique<I>...>;
};
#else
#include <tuple>    

template<typename TupleA, typename TupleB>
constexpr auto operator+(const tuple_catter<TupleA>& a, const tuple_catter<TupleB>& b)
{
    return tuple_catter{::std::tuple_cat(a.t, b.t)};
}

template<typename TupleA, typename X>
constexpr auto operator+(const tuple_catter<TupleA>& a, const X& x)
{
    return tuple_catter{::std::tuple_cat(a.t, ::std::tuple{x})};
}

constexpr ::std::tuple empty_tuple {};

template<::std::size_t... I>
struct make_test_tuple<::std::index_sequence<I...>>
{
    using type = ::std::tuple<Unique<I>...>;
};
#endif // YYMP_STRESS_STD_TUPLE

// Synthesizes an expression that effectively joins many tuples through a 
// concatenation method, one pair at a time.
// While one can and should join them all at once using a single call to the 
// concatenation function, this is not always possible.
template<::std::size_t... I>
constexpr
auto test(::std::integer_sequence<::std::size_t, I...>)
{
    return (tuple_catter{empty_tuple} + ... + Unique<I>{});
}

constexpr auto foo = test(::std::make_index_sequence<TEST_DEPTH + 1>{}).t;

// Verify the tuple type that results from the test.
static_assert(std::is_same_v<
    decltype(foo),
    const typename make_test_tuple<::std::make_index_sequence<TEST_DEPTH + 1>>::type
>);
    

int main()
{
    
}