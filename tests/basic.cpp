//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/* Test cases:
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
 * class_type_parameters
 *   class that is not instantiated from a template             result is empty typelist
 *   class that is instantiated from template of type params    result is a typelist of type params
 */

#include <concepts>
#include <type_traits>

#include <yymp/typelist.hpp>

using namespace yymp;
using ::std::same_as;
using ::std::index_sequence;

struct custom;
template< class... > struct Template;

/* **********************
 * Categorization traits:
 *   is_typelist,
 *   is_empty_typelist,
 *   is_nonempty_typelist
 */

static_assert(is_typelist_v<typelist<>>);
static_assert(is_empty_typelist_v<typelist<>>);
static_assert(!is_nonempty_typelist_v<typelist<>>);

static_assert(is_typelist_v<typelist<custom>>);
static_assert(!is_empty_typelist_v<typelist<custom>>);
static_assert(is_nonempty_typelist_v<typelist<custom>>);

static_assert(is_typelist_v<typelist<void, custom>>);
static_assert(!is_empty_typelist_v<typelist<void, custom>>);
static_assert(is_nonempty_typelist_v<typelist<void, custom>>);

static_assert(!is_typelist_v<custom>);
static_assert(!is_empty_typelist_v<custom>);
static_assert(!is_nonempty_typelist_v<custom>);

/* **************
 * typelist_first
 */

static_assert(!requires { typename typelist_first_t<typelist<>>; });
static_assert(same_as<typelist_first_t<typelist<custom>>, custom>);
static_assert(same_as<typelist_first_t<typelist<custom, void, char>>, custom>);

/* ******************
 * retain_as_typelist
 */

static_assert(same_as<retain_as_typelist_t<typelist<>>, typelist<>>);
static_assert(same_as<retain_as_typelist_t<typelist<custom>>, typelist<custom>>);
static_assert(same_as<retain_as_typelist_t<custom, typelist<custom>>);

/* *************
 * typelist_size
 */

static_assert(typelist_size_v<typelist<>> == 0);
static_assert(typelist_size_v<typelist<custom>> == 1);
static_assert(typelist_size_v<typelist<int, void, custom>> == 3);
static_assert(typelist_size_v<typelist<custom, custom, custom>> == 3);

/* ************************
 * template_type_parameters
 */

static_assert(same_as<template_type_parameters_t<custom>, typelist<>>);
static_assert(same_as<
    template_type_parameters_t<Template<int, char, void>>, 
    typelist<int, char, void>
>);

/* ************
 * typelist_get
 */

static_assert(same_as<typelist_get_t<0, typelist<int, char, custom>>, int>);
static_assert(same_as<typelist_get_t<1, typelist<int, char, custom>>, char>);
static_assert(same_as<typelist_get_t<2, typelist<int, char, custom>>, custom>);

/* ***************
 * typelist_select
 */

static_assert(same_as<
    typelist_select_t<index_sequence<0, 1, 2>, typelist<int, char, custom>>,
    typelist<int, char, custom>
>);
static_assert(same_as<
    typelist_select_t<index_sequence<2, 0, 1>, typelist<int, char, custom>>,
    typelist<custom, int, char>
>);
static_assert(same_as<
    typelist_select_t<index_sequence<1, 2, 0>, typelist<int, char, custom>>,
    typelist<char, custom, int>
>);
static_assert(same_as<
    typelist_select_t<index_sequence<0, 2, 1>, typelist<int, char, custom>>,
    typelist<int, custom, char>
>);
static_assert(same_as<
    typelist_select_t<index_sequence<1, 0, 2>, typelist<int, char, custom>>,
    typelist<char, int, custom>
>);
static_assert(same_as<
    typelist_select_t<index_sequence<2, 1, 0>, typelist<int, char, custom>>,
    typelist<custom, char, int>
>);

static_assert(same_as<
    typelist_select_t<index_sequence<0, 1>, typelist<int, char, custom>>,
    typelist<int, char>
>);
static_assert(same_as<
    typelist_select_t<index_sequence<0, 2>, typelist<int, char, custom>>,
    typelist<int, custom>
>);
static_assert(same_as<
    typelist_select_t<index_sequence<1, 0>, typelist<int, char, custom>>,
    typelist<char, int>
>);
static_assert(same_as<
    typelist_select_t<index_sequence<1, 2>, typelist<int, char, custom>>,
    typelist<char, custom>
>);
static_assert(same_as<
    typelist_select_t<index_sequence<2, 0>, typelist<int, char, custom>>,
    typelist<custom, int>
>);
static_assert(same_as<
    typelist_select_t<index_sequence<2, 1>, typelist<int, char, custom>>,
    typelist<custom, char>
>);

static_assert(same_as<
    typelist_select_t<index_sequence<0>, typelist<int, char, custom>>,
    typelist<int>
>);
static_assert(same_as<
    typelist_select_t<index_sequence<1>, typelist<int, char, custom>>,
    typelist<char>
>);
static_assert(same_as<
    typelist_select_t<index_sequence<2>, typelist<int, char, custom>>,
    typelist<custom>
>);

static_assert(same_as<
    typelist_select_t<index_sequence<>, typelist<int, char, custom>>,
    typelist<>
>);

/* *************
 * typelist_join
 */

static_assert(same_as<typelist_join_t<>, typelist<>>);
static_assert(same_as<typelist_join_t<typelist<int>>, typelist<int>>);
static_assert(same_as<
    typelist_join_t<typelist<int>, typelist<char>>,
    typelist<int, char>
>);

static_assert(same_as<
    typelist_join_t<
        typelist<int>,
        typelist<char>,
        typelist<custom>
    >, typelist<int, char, custom>
>);

static_assert(same_as<
    typelist_join_t<
        typelist<>, typelist<>,
        typelist<int>, typelist<>, typelist<>,
        typelist<char>, typelist<>, typelist<>,
        typelist<custom> typelist<>, typelist<>
    >, typelist<int, char, custom>
>);

/* ***************
 * typelist_append
 */

static_assert(same_as<typelist_append_t<typelist<>, int>, typelist<int>>);
static_assert(same_as<typelist_append_t<typelist<int>, char>, typelist<int, char>>);

/* ******************
 * typelist_transform
 */

static_assert(same_as<
    typelist_transform_t<std::make_pointer, typelist<int, char, custom>>,
    typelist<int*, char*, custom*>
>);

static_assert(same_as<
    typelist_expand_t<Template, typelist<int, char, custom>>,
    Template<int, char, custom>
>);

/* ***************
 * typelist_all_of/typelist_any_of/typelist_none_of/typelist_count_of
 */

static_assert(typelist_all_of_v<custom, typelist<>>);
static_assert(!typelist_all_of_v<custom, typelist<int, char, custom>);
static_assert(!typelist_all_of_v<void, typelist<int, char, custom>);

static_assert(!typelist_any_of_v<custom, typelist<>>);
static_assert(typelist_any_of_v<custom, typelist<int, char, custom>);
static_assert(!typelist_any_of_v<void, typelist<int, char, custom>);

static_assert(typelist_none_of_v<custom, typelist<>>);
static_assert(!typelist_none_of_v<custom, typelist<int, char, custom>);
static_assert(typelist_none_of_v<void, typelist<int, char, custom>);

static_assert(typelist_count_of_v<custom, typelist<>> == 0);
static_assert(typelist_count_of_v<custom, typelist<int, char, custom>> == 1);
static_assert(typelist_count_of_v<custom, typelist<int, custom, custom>> == 2);
static_assert(typelist_count_of_v<void, typelist<int, char, custom>> == 0);

/* ***************
 * typelist_all_where/typelist_any_where/typelist_none_where/typelist_count_where
 */

static_assert(typelist_all_where_v<std::is_pointer, typelist<>>);
static_assert(typelist_all_where_v<std::is_pointer, typelist<int*, char*, custom*>);
static_assert(!typelist_all_where_v<std::is_pointer, typelist<int*, char*, custom>);
static_assert(!typelist_all_where_v<std::is_pointer, typelist<int, char, custom>);

static_assert(!typelist_any_where_v<std::is_pointer, typelist<>>);
static_assert(typelist_any_where_v<std::is_pointer, typelist<int*, char*, custom*>);
static_assert(typelist_any_where_v<std::is_pointer, typelist<int*, char*, custom>);
static_assert(!typelist_any_where_v<std::is_pointer, typelist<int, char, custom>);

static_assert(typelist_none_where_v<std::is_pointer, typelist<>>);
static_assert(!typelist_none_where_v<std::is_pointer, typelist<int*, char*, custom*>);
static_assert(!typelist_none_where_v<std::is_pointer, typelist<int*, char*, custom>);
static_assert(typelist_none_where_v<std::is_pointer, typelist<int, char, custom>);

static_assert(typelist_count_where_v<std::is_pointer, typelist<>> == 0);
static_assert(typelist_count_where_v<std::is_pointer, typelist<int*, char*, custom*>> == 3);
static_assert(typelist_count_where_v<std::is_pointer, typelist<int*, char, custom*>> == 2);
static_assert(typelist_count_where_v<std::is_pointer, typelist<int, char, custom>> == 0);

/* *******************
 * typelist_indices_of
 */

static_assert(same_as<
    typelist_indices_of_t<custom, typelist<>>,
    index_sequence<>
>);
static_assert(same_as<
    typelist_indices_of_t<custom, typelist<int, char>>,
    index_sequence<>
>);
static_assert(same_as<
    typelist_indices_of_t<custom, typelist<custom, int, char>>,
    index_sequence<0>
>);
static_assert(same_as<
    typelist_indices_of_t<custom, typelist<int, custom, char>>,
    index_sequence<1>
>);
static_assert(same_as<
    typelist_indices_of_t<custom, typelist<int, char, custom>>,
    index_sequence<2>
>);
static_assert(same_as<
    typelist_indices_of_t<custom, typelist<custom, int, custom>>,
    index_sequence<0, 2>
>);

/* **********************
 * typelist_indices_where
 */

static_assert(same_as<
    typelist_indices_where_t<std::is_pointer, typelist<>>,
    index_sequence<>
>);
static_assert(same_as<
    typelist_indices_where_t<std::is_pointer, typelist<int, char>>,
    index_sequence<>
>);
static_assert(same_as<
    typelist_indices_where_t<std::is_pointer, typelist<custom*, int, char>>,
    index_sequence<0>
>);
static_assert(same_as<
    typelist_indices_where_t<std::is_pointer, typelist<int, custom*, char>>,
    index_sequence<1>
>);
static_assert(same_as<
    typelist_indices_where_t<std::is_pointer, typelist<int, char, custom*>>,
    index_sequence<2>
>);
static_assert(same_as<
    typelist_indices_where_t<std::is_pointer, typelist<int*, custom*, char>>,
    index_sequence<0, 1>
>);

/* ***************
 * typelist_filter
 */

static_assert(same_as<
    typelist_filter_t<std::is_pointer, typelist<>>,
    typelist<>
>);
static_assert(same_as<
    typelist_filter_t<std::is_pointer, typelist<int, char, custom>>,
    typelist<>
>);
static_assert(same_as<
    typelist_filter_t<std::is_pointer, typelist<int*, char, custom>>,
    typelist<int*>
>);
static_assert(same_as<
    typelist_filter_t<std::is_pointer, typelist<int, char*, custom>>,
    typelist<char*>
>);
static_assert(same_as<
    typelist_filter_t<std::is_pointer, typelist<int, char, custom*>>,
    typelist<custom*>
>);
static_assert(same_as<
    typelist_filter_t<std::is_pointer, typelist<int*, char, custom*>>,
    typelist<int*, custom*>
>);
static_assert(same_as<
    typelist_filter_t<std::is_pointer, typelist<int*, char*, custom>>,
    typelist<int*, char*>
>);
static_assert(same_as<
    typelist_filter_t<std::is_pointer, typelist<int, char*, custom*>>,
    typelist<char*, custom*>
>);
static_assert(same_as<
    typelist_filter_t<std::is_pointer, typelist<int*, char*, custom*>>,
    typelist<int*, char*, custom*>
>);

/* **************************
 * typelist_filter_duplicates
 */

static_assert(same_as<
    typelist_filter_duplicates_t<typelist<>>,
    typelist<>
>);
static_assert(same_as<
    typelist_filter_duplicates_t<typelist<int, char, custom>>,
    typelist<int, char, custom>
>);
static_assert(same_as<
    typelist_filter_duplicates_t<typelist<int, int, char, char, custom, custom>>,
    typelist<int, char, custom>
>);
static_assert(same_as<
    typelist_filter_duplicates_t<typelist<int, char, int, custom, char, custom>>,
    typelist<int, char, custom>
>);

/* *****************
 * typelist_group_by
 */

static_assert(same_as<
    typelist_group_by_t<std::is_pointer, typelist<>>,
    typelist<>
>);
static_assert(same_as<
    typelist_group_by_t<std::is_pointer, typelist<int, char, custom*>>,
    typelist<typelist<int, char>, typelist<custom*>>
>);
static_assert(same_as<
    typelist_group_by_t<std::is_pointer, typelist<int*, char, custom>>,
    typelist<typelist<int*>, typelist<char, custom>>
>);