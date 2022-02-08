// SPDX-License-Identifier: BSL-1.0

#include <cstdio>

#include <concepts>
#include <type_traits>

#include <yymp/wref_tuple.hpp>
#include <yymp/stuple.hpp>

#include <tuple>

using ::yymp::stuple;
using ::yymp::wref_tuple;

using std::tuple_size;
using std::tuple_element;
using std::same_as;

// =============================================================================
// Construct empty tuples

constexpr wref_tuple empty_wtuple;
constexpr stuple     empty_stuple;

static_assert(tuple_size<decltype(empty_wtuple)>::value == 0);
static_assert(tuple_size<decltype(empty_stuple)>::value == 0);

// =============================================================================
// Construct singletons

constexpr stuple     single_stuple {0xF00D}; // stuple<int>
constexpr wref_tuple single_wtuple {single_stuple}; 

static_assert(get<0>(single_stuple) == 0xF00D);
static_assert(same_as<decltype(get<0>(single_stuple)), const int&>);

static_assert(get<0>(single_wtuple) == 0xF00D);
static_assert(same_as<decltype(get<0>(single_wtuple)), const int&>);

// =============================================================================
// Construct wide tuple from mix of empty tuples

// more complicated wtuple
constexpr wref_tuple multiple_empty_wtuple
    {empty_wtuple, empty_stuple, single_stuple, empty_wtuple, empty_stuple};

static_assert(get<0>(multiple_empty_wtuple) == 0xF00D);
static_assert(tuple_size<decltype(multiple_empty_wtuple)>::value == 1);

// =============================================================================
// Construct wide tuple from multiple identical tuples

constexpr wref_tuple multiple_identical_wtuples
    {multiple_empty_wtuple, multiple_empty_wtuple, multiple_empty_wtuple};

static_assert(get<0>(multiple_identical_wtuples) == 0xF00D);
static_assert(get<1>(multiple_identical_wtuples) == 0xF00D);
static_assert(get<2>(multiple_identical_wtuples) == 0xF00D);
static_assert(tuple_size<decltype(multiple_identical_wtuples)>::value == 3);

// =============================================================================
// Complex stuple (1)

constexpr stuple complex_stuple1 {5, "foo", 0.5f};
using complex_stuple1_type = std::remove_cvref_t<decltype(complex_stuple1)>;

static_assert(get<0>(complex_stuple1) == 5);
static_assert(get<1>(complex_stuple1) == "foo");
static_assert(get<2>(complex_stuple1) == 0.5f);
static_assert(same_as<decltype(get<0>(complex_stuple1)), const int&>);
static_assert(same_as<decltype(get<1>(complex_stuple1)), const char* const&>);
static_assert(same_as<decltype(get<2>(complex_stuple1)), const float&>);
static_assert(same_as<typename tuple_element<0, complex_stuple1_type>::type, int>);
static_assert(same_as<typename tuple_element<1, complex_stuple1_type>::type, const char*>);
static_assert(same_as<typename tuple_element<2, complex_stuple1_type>::type, float>);

// =============================================================================
// test stuple_cat, empty

static_assert(same_as<decltype(yymp::stuple_cat()), stuple<>>);

// =============================================================================
// test stuple_cat, single tuple

static_assert(same_as<decltype(yymp::stuple_cat(std::make_tuple(5, 'c'))), stuple<int, char>>);

// =============================================================================
// test stuple_cat, complex

static_assert(same_as<
    decltype(yymp::stuple_cat(
        single_stuple,
        complex_stuple1,
        stuple{5L},
        complex_stuple1
    )),
    stuple<
        int, 
        int, const char*, float,
        long, 
        int, const char*, float
    >
>);

int main()
{
    return 0;
}