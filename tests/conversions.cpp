//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <yymp/typelist_fwd.hpp>
#include <yymp/conversions.hpp>

#include <type_traits> // std::is_same
#include <utility> // std::integer_sequence

using yymp::typelist;
using yymp::as_typelist;

using std::is_same;

struct T;

///////////////////////
// as_typelist

static_assert(is_same<typename as_typelist<typelist<T>>::type, typelist<T>>(), "as_typelist must map a typelist to itself");
static_assert(is_same<typename as_typelist<T>::type, typelist<T>>(), "as_typelist must map a non-typelist T to a singleton typelist containing T");

///////////////////////
// typelist <-> integer_sequence

#if __cpp_lib_integer_sequence >= 201304


using yymp::integer_sequence_to_typelist;
using yymp::typelist_to_integer_sequence;

using seq = std::index_sequence<1,5,3,0,1,4>;
using seq_as_typelist = typelist<
    std::integral_constant<std::size_t, 1>,
    std::integral_constant<std::size_t, 5>,
    std::integral_constant<std::size_t, 3>,
    std::integral_constant<std::size_t, 0>,
    std::integral_constant<std::size_t, 1>,
    std::integral_constant<std::size_t, 4>
>;

static_assert(is_same<typename integer_sequence_to_typelist<seq>::type, seq_as_typelist>(), "converted integer sequence must match the reference answer");
static_assert(is_same<typename typelist_to_integer_sequence<seq_as_typelist>::type, seq>(), "converted integer sequence as typelist must match the reference answer");
#endif 