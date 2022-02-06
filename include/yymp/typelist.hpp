// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_TYPELIST_HPP
#define YYMP_TYPELIST_HPP

#include <cstddef>

#include <array>
#include <type_traits>
#include <utility>

#include <yymp/utility.hpp>

#include <yymp/dtl/typelist_accumulation.hpp>
#include <yymp/dtl/typelist_deductions.hpp>
#include <yymp/dtl/typelist_indexing.hpp>

namespace yymp
{
    template<typename... Types>
    struct typelist
    {
        using type = typelist;
        
        static constexpr ::std::size_t size = sizeof...(Types);
    };
    
    /**
     * \brief A `UnaryTypeTrait` that determines if \a T is a #typelist.
     */
    template<class T>
    struct is_typelist;
    
    /**
     * \brief A constraint which passes if and only if \a T is a #typelist 
     *        specialization.
     */
    template<typename T>
    concept typelist_specialization = is_typelist<T>::value;
    
    /**
     * \brief A `UnaryTypeTrait` that determines if \a T is an empty #typelist.
     */
    template<class T>
    struct is_empty_typelist;
    
    /**
     * \brief A `UnaryTypeTrait` that determines if \a T is an non-empty 
     *       #typelist.
     */
    template<class T>
    struct is_nonempty_typelist;
    
    /**
     * \brief A `TransformationTrait` providing `type` as the first type in 
     *        \a TypeList.
     *
     * This template is SFINAE-friendly; `type` is not provided if \a TypeList 
     * is not a #typelist or is an empty #typelist.
     */
    template<class TypeList>
    struct typelist_first;
    
    /**
     * \brief A `TransformationTrait` providing `type` as \a T if \a T is a 
     *        #typelist, or otherwise as `typelist<T>`.
     */
    template<typename T>
    struct retain_as_typelist;
    
    /**
     * \brief A `UnaryTypeTrait` that determines the size of \a TypeList.
     *
     * \tparam TypeList The #typelist.
     */
    template<class TypeList>
    struct typelist_size;
    
    /**
     * \brief A `TransformationTrait` providing `type` as a #typelist of the
     *        type parameters for some template specialization \a T.
     *
     * If \a T is not of the form `Template<TypeArgs...>` where `Template` is 
     * the class template used to instantiate \a T and `TypeArgs` are the 
     * type template arguments to `Template`, then the result `type` is 
     * `typelist<>`.
     */
    template<class T>
    struct template_type_parameters;
    
    /**
     * \brief A `TransformationTrait` providing `type` as the `I`-th type in 
     *        \a TypeList, in 0-based fashion.
     *
     * \tparam I        The index of the type.
     * \tparam TypeList The #typelist to index into.
     */
    template<::std::size_t I, class TypeList>
        requires (is_typelist<TypeList>::value && I < TypeList::size)
    struct typelist_get;
    
    /**
     * \brief A `TransformationTrait` providing `type` as a #typelist of types 
     *        in \a TypeList, selected by the indices in \a IndexSequence.
     *
     * \tparam IndexSequence The `std::index_sequence` of the indices to select.
     * \tparam TypeList The #typelist to select from.
     */
    template<class IndexSequence, class TypeList>
    struct typelist_select;
    
    /**
     * \brief A `TransformationTrait` providing `type` as the concatenation of 
     *        multiple \a TypeLists into a single #typelist.
     *
     * \tparam TypeLists The #typelist specializations to concatenate.
     */
    template<class... TypeLists>
        requires (is_typelist<TypeLists>::value && ...)
    struct typelist_join;
    
    /**
     * \brief A `TransformationTrait` providing `type` as a #typelist that is 
     *        \a TypeList with \a T appended to the end.
     *
     * \tparam TypeList The #typelist.
     * \tparam T        The type to append to \a TypeList.
     */
    template<class TypeList, typename T>
    struct typelist_append;
    
    /**
     * \brief A `TransformationTrait` providing `type` as a #typelist where each
     *        type `T` in \a TypeList becomes `TransformationTrait<T>::type`.
     *
     * \tparam TransformationTrait The transformation.
     * \tparam TypeList            The #typelist to transform.
     */
    template<template<typename...> class TransformationTrait, class TypeList>
    struct typelist_transform;
    
    /**
     * \brief A `TransformationTrait` providing `type` as `Template<Types...>`
     *        where \a TypeList is `typelist<Types...>`.
     *
     * \tparam Template The class template to expand.
     * \tparam TypeList The #typelist of the types to expand with.
     */
    template<template<typename...> class Template, class TypeList>
    struct typelist_expand;

    /**
     * \brief A `TransformationTrait` providing `type` as 
     *        `Template<Types...>::type` where \a TypeList is 
     *        `typelist<Types...>`.
     *
     * \tparam TransformationTrait The trait to expand into.
     * \tparam TypeList            The #typelist of the types to expand with.
     */
    template<template<typename...> class TransformationTrait, class TypeList>
    struct typelist_expand_trait;
    
    /**
     * \brief A `TransformationTrait` providing `type` as a left-to-right 
     *        binary accumulation of types in \a TypeList.
     *
     * \tparam BinaryAccumulator The `TransformationTrait` for type composition.
     * \tparam Init              The initial type.
     * \tparam TypeList          The #typelist of types to accumulate through.
     */
    template<
        template<typename...> class BinaryAccumulator, 
        typename Init,
        class TypeList
    > struct typelist_accumulate;
    
    /**
     * \brief A `BinaryTypeTrait` of value type `bool` for which `value` is 
     *        `true` if and only if all the types in \a TypeList are \a T.
     *
     * \tparam T        The type to find.
     * \tparam TypeList The #typelist to search.
     */
    template<typename T, class TypeList>
    struct typelist_all_of;
    
    /**
     * \brief A `BinaryTypeTrait` of value type `bool` for which `value` is 
     *        `true` if and only if any type in \a TypeList is \a T.
     *
     * \tparam T        The type to find.
     * \tparam TypeList The #typelist to search.
     */
    template<typename T, class TypeList>
    struct typelist_any_of;
    
    /**
     * \brief A `BinaryTypeTrait` of value type `bool` for which `value` is 
     *        `true` if and only if no type in \a TypeList is \a T.
     *
     * \tparam T        The type to find.
     * \tparam TypeList The #typelist to search.
     */
    template<typename T, class TypeList>
    struct typelist_none_of;
    
    /**
     * \brief A `BinaryTypeTrait`` of value type `bool` for which `value` is 
     *        `true` if and only if all types in \a TypeList satisfy 
     *        `UnaryTypeTrait`.
     * 
     * Satisfaction of a type `T` with \a UnaryTypeTrait is determined by 
     * `std::is_same<typename UnaryTypeTrait<T>::type, std::true_type>::value`.
     *
     * \tparam UnaryTypeTrait The trait providing the condition to check.
     * \tparam TypeList       The #typelist to search.
     */
    template<template<typename...> class UnaryTypeTrait, class TypeList>
    struct typelist_all_where;
    
    /**
     * \brief A `BinaryTypeTrait`` of value type `bool` for which `value` is 
     *        `true` if and only if all types in \a TypeList satisfy 
     *        `UnaryTypeTrait`.
     * 
     * Satisfaction of a type `T` with \a UnaryTypeTrait is determined by 
     * `std::is_same<typename UnaryTypeTrait<T>::type, std::true_type>::value`.
     *
     * \tparam UnaryTypeTrait The trait providing the condition to check.
     * \tparam TypeList       The #typelist to search.
     */
    template<template<typename...> class UnaryTypeTrait, class TypeList>
    struct typelist_any_where;
    
    /**
     * \brief A `BinaryTypeTrait`` of value type `bool` for which `value` is 
     *        `true` if and only if all types in \a TypeList satisfy 
     *        `UnaryTypeTrait`.
     * 
     * Satisfaction of a type `T` with \a UnaryTypeTrait is determined by 
     * `std::is_same<typename UnaryTypeTrait<T>::type, std::true_type>::value`.
     *
     * \tparam UnaryTypeTrait The trait providing the condition to check.
     * \tparam TypeList       The #typelist to search.
     */
    template<template<typename...> class UnaryTypeTrait, class TypeList>
    struct typelist_none_where;
    
    /**
     * \brief A `BinaryTypeTrait` for which `value` is a `std::size_t` of the 
     *        number of occurrences of \a T in \a TypeList.
     *
     * \tparam T        The type to find.
     * \tparam TypeList The #typelist to search.
     */
    template<typename T, class TypeList>
    struct typelist_count_of;
    
    /**
     * \brief A `BinaryTypeTrait` for which `value` is a `std::size_t` of the 
     *        number of types which satisfy \a UnaryTypeTrait.
     *
     * Satisfaction of a type `T` with \a UnaryTypeTrait is determined by 
     * `std::is_same<typename UnaryTypeTrait<T>::type, std::true_type>::value`.
     *
     * \tparam UnaryTypeTrait The trait used to find types.
     * \tparam TypeList       The #typelist to search.
     */
    template<template<typename...> class UnaryTypeTrait, class TypeList>
        requires is_typelist<TypeList>::value
    struct typelist_count_where;
    
    /**
     * \brief A `TransformationTrait` providing `type` as a 
     *        `std::index_sequence` of the indices of \a T in \a TypeList.
     *
     * \tparam T        The type to search for.
     * \tparam TypeList The #typelist.
     */
    template<typename T, class TypeList>
        requires is_typelist<TypeList>::value
    struct typelist_indices_of;
    
    /**
     * \brief A `TransformationTrait` providing `type` as a 
     *        `std::index_sequence` of the indices of types in \a TypeList 
     *        which satisfy \a UnaryTypeTrait.
     *
     * Satisfaction of a type `T` with \a UnaryTypeTrait is determined by 
     * `std::is_same<typename UnaryTypeTrait<T>::type, std::true_type>::value`.
     *
     * \tparam UnaryTypeTrait The trait used to find types.
     * \tparam TypeList       The #typelist.
     */
    template<template<typename...> class UnaryTypeTrait, class TypeList>
        requires is_typelist<TypeList>::value
    struct typelist_indices_where;
    
    /**
     * \brief A `TransformationTrait` providing `type` as a #typelist consisting
     *        of all the types `T` in \a TypeList which satisfy 
     *        \a UnaryTypeTrait.
     *
     * Satisfaction of a type `T` with \a UnaryTypeTrait is determined by 
     * `std::is_same<typename UnaryTypeTrait<T>::type, std::true_type>::value`.
     *
     * \tparam UnaryTypeTrait The trait to filter types.
     * \tparam TypeList       The #typelist to filter.
     */
    template<template<typename...> class UnaryTypeTrait, class TypeList>
        requires is_typelist<TypeList>::value
    struct typelist_filter;
    
    /**
     * \brief A `TransformationTrait` providing `type` as a #typelist of types 
     *        from \a TypeList, filtered to remove duplicate types.
     *
     * The order of first occurrence of types in `type` and \a TypeList is
     * preserved.
     */
    template<class TypeList>
        requires is_typelist<TypeList>::value
    struct typelist_filter_duplicates;
    
    /**
     * \brief A `TransformationTrait` providing `type` as a #typelist of 
     *        #typelist specializations, each of which consists of the types 
     *        of \a TypeList which map to the same type through 
     *        \a TransformationTrait.
     */
    template<template<typename...> class TransformationTrait, class TypeList>
        requires is_typelist<TypeList>::value
    struct typelist_group_by;
    
    // =========================================================================
    // Convenience aliases and variable templates
    
    template<class T>
    inline constexpr bool is_typelist_v = is_typelist<T>::value;
    
    template<class T>
    inline constexpr bool is_empty_typelist_v = is_empty_typelist<T>::value;
    
    template<class T>
    inline constexpr bool is_nonempty_typelist_v 
        = is_nonempty_typelist<T>::value;
    
    template<class TypeList>
    inline constexpr ::std::size_t typelist_size_v
        = typelist_size<TypeList>::value;
    
    template<class TypeList>
    using typelist_first_t = typename typelist_first<TypeList>::type;
    
    template<typename T>
    using retain_as_typelist_t = typename retain_as_typelist<T>::type;
    
    template<class T>
    using template_type_parameters_t 
        = typename template_type_parameters<T>::type;
    
    template<::std::size_t I, class TypeList>
    using typelist_get_t = typename typelist_get<I, TypeList>::type;
    
    template<class IndexSequence, class TypeList>
    using typelist_select_t 
        = typename typelist_select<IndexSequence, TypeList>::type;
    
    template<class... TypeLists>
    using typelist_join_t = typename typelist_join<TypeLists...>::type;
    
    template<class TypeList, typename T>
    using typelist_append_t 
        = typename typelist_append<TypeList, T>::type;
    
    template<template<typename...> class TransformationTrait, class TypeList>
    using typelist_transform_t 
        = typename typelist_transform<TransformationTrait, TypeList>::type;
        
    template<template<typename...> class Template, class TypeList>
    using typelist_expand_t
        = typename typelist_expand<Template, TypeList>::type;
    
    template<template<typename...> class TransformationTrait, class TypeList>
    using typelist_expand_trait_t
        = typename typelist_expand_trait<TransformationTrait, TypeList>::type;
    
    template<
        template<typename...> class BinaryAccumulator,
        typename Init,
        class TypeList
    > using typelist_accumulate_t
        = typename typelist_accumulate<BinaryAccumulator, Init, TypeList>::type;
    
    template<typename T, class TypeList>
    inline constexpr bool typelist_all_of_v 
        = typelist_all_of<T, TypeList>::value;
        
    template<typename T, class TypeList>
    inline constexpr bool typelist_any_of_v
        = typelist_any_of<T, TypeList>::value;
    
    template<typename T, class TypeList>
    inline constexpr bool typelist_none_of_v
        = typelist_none_of<T, TypeList>::value;
    
    template<template<typename...> class UnaryTypeTrait, class TypeList>
    inline constexpr bool typelist_all_where_v
        = typelist_all_where<UnaryTypeTrait, TypeList>::value;
    
    template<template<typename...> class UnaryTypeTrait, class TypeList>
    inline constexpr bool typelist_any_where_v
        = typelist_any_where<UnaryTypeTrait, TypeList>::value;
    
    template<template<typename...> class UnaryTypeTrait, class TypeList>
    inline constexpr bool typelist_none_where_v
        = typelist_none_where<UnaryTypeTrait, TypeList>::value;
    
    template<typename T, class TypeList>
    inline constexpr ::std::size_t typelist_count_of_v
        = typelist_count_of<T, TypeList>::value;
    
    template<template<typename...> class UnaryTypeTrait, class TypeList>
    inline constexpr ::std::size_t typelist_count_where_v
        = typelist_count_where<UnaryTypeTrait, TypeList>::value;
    
    template<typename T, class TypeList>
    using typelist_indices_of_t
        = typename typelist_indices_of<T, TypeList>::type;
    
    template<template<typename...> class UnaryTypeTrait, class TypeList>
    using typelist_indices_where_t
        = typename typelist_indices_where<UnaryTypeTrait, TypeList>::type;
    
    template<template<typename...> class UnaryTypeTrait, class TypeList>
    using typelist_filter_t
        = typename typelist_filter<UnaryTypeTrait, TypeList>::type;
    
    template<class TypeList>
    using typelist_filter_duplicates_t
        = typename typelist_filter_duplicates<TypeList>::type;
        
    template<template<typename...> class TransformationTrait, class TypeList>
    using typelist_group_by_t
        = typename typelist_group_by<TransformationTrait, TypeList>::type;
    
    // =========================================================================
    // Concepts
    
    /**
     * \brief A constraint which passes if and only if there is exactly one 
     *        occurrence of \a Type in \a TypeList.
     *
     * \tparam Type     The type to check for.
     * \tparam TypeList The #typelist to check within.
     */
    template<typename Type, typename TypeList>
    concept unique_in_typelist =
        is_typelist<TypeList>::value &&
        requires (dtl::typelist_deductions::deduction_list<0, TypeList> list)
        {
            {::yymp::dtl::typelist_deductions::deduce_index<Type>(list)};
        };
    
    /**
     * \brief A constraint which passes if and only if \a Type occurs at least
     *        once in \a TypeList.
     *
     * \tparam Type     The type to check for.
     * \tparam TypeList The #typelist to check within.
     */
    template<typename Type, typename TypeList>
    concept any_in_typelist =
        is_typelist<TypeList>::value && typelist_any_of<Type, TypeList>::value;
    
    /**
     * \brief A constraint which passes if and only if there is exactly one 
     *        occurrence of \a Type among \a Types.
     *
     * \tparam Type  The type to check for.
     * \tparam Types The types to check within.
     */
    template<typename Type, typename... Types>
    concept unique_among = unique_in_typelist<Type, typelist<Types...>>;
    
    /**
     * \brief A constraint which passes if and only if \a Type occurs at least 
     *        once among \a Types.
     *
     * \tparam Type  The type to check for.
     * \tparam Types The types to check within.
     */
    template<typename Type, typename... Types>
    concept any_among = any_in_typelist<Type, typelist<Types...>>;
    
    // =========================================================================
    // Functions
    
    /**
     * \brief Calls `f(type_marker<T>{})` for each type `T` in \a Types.
     *
     * \tparam     Types The types to loop over.
     * \param [in] f     The function to call.
     * 
     * \return `std::move(f)`
     */
    template<typename... Types, class UnaryFunction>
    constexpr
    UnaryFunction for_each(typelist<Types...>, UnaryFunction f);
    
    // =========================================================================
    // IMPLEMENTATION
    
    template<class T>
    struct is_typelist : std::false_type { };
    
    template<typename... Types>
    struct is_typelist<typelist<Types...>> : std::true_type { };
    
    template<class T>
    struct is_empty_typelist : std::false_type { };
    
    template<>
    struct is_empty_typelist<typelist<>> : std::true_type { };
    
    template<class T>
    struct is_nonempty_typelist : std::false_type { };
    
    template<typename... Types>
    struct is_nonempty_typelist<typelist<Types...>>
        : std::bool_constant<sizeof...(Types) != 0> { };
    
    template<>
    struct typelist_first<typelist<>> { };
    
    template<typename First, typename... Rest>
    struct typelist_first<typelist<First, Rest...>> { using type = First; };
    
    template<typename T>
    struct retain_as_typelist { using type = typelist<T>; };
    
    template<typename... Types>
    struct retain_as_typelist<typelist<Types...>>
    { using type = typelist<Types...>; };
    
    template<typename... Types>
    struct typelist_size<typelist<Types...>>
        : std::integral_constant<::std::size_t, sizeof...(Types)> { };
    
    template<class T>
    struct template_type_parameters { using type = typelist<>; };
    
    template<template <class...> class Template, typename... Types>
    struct template_type_parameters<Template<Types...>>
    { using type = typelist<Types...>; };
    
    template<::std::size_t I, class TypeList>
        requires (is_typelist<TypeList>::value && I < TypeList::size)
    struct typelist_get
    {
        using type = decltype(::yymp::dtl::typelist_deductions::deduce_type<I>(
            ::yymp::dtl::typelist_deductions::deduction_list<0, TypeList>{}
        ));
    };
    
    template<::std::size_t... Is, typename... Types>
    struct typelist_select<::std::index_sequence<Is...>, typelist<Types...>>
    {
        using type = typelist<
            typename typelist_get<Is, typelist<Types...>>::type...
        >;
    };
    
    template<>
    struct typelist_join<> { using type = typelist<>; };
    
    template<typename... Types>
    struct typelist_join<typelist<Types...>>
    {
        using type = typelist<Types...>;
    };
    
    template<typename... ATypes, typename... BTypes>
    struct typelist_join<typelist<ATypes...>, typelist<BTypes...>>
    {
        using type = typelist<ATypes..., BTypes...>;
    };
    
    template<class... TypeLists>
        requires (is_typelist<TypeLists>::value && ...)
    struct typelist_join
    {
        using type = typename ::yymp::dtl::typelist_deductions::join_impl<
            typelist<TypeLists...>
        >::type;
    };
    
    template<typename... Types, typename T>
    struct typelist_append<typelist<Types...>, T>
    {
        using type = typelist<Types..., T>;
    };
   
    
    template<template<typename...> class TransformationTrait, typename... Types>
    struct typelist_transform<TransformationTrait, typelist<Types...>>
    { using type = typelist<typename TransformationTrait<Types>::type...>; };
    
    template<template<typename...> class Template, typename... Types>
    struct typelist_expand<Template, typelist<Types...>>
    { using type = Template<Types...>; };
    
    template<template<typename...> class TransformationTrait, typename... Types>
    struct typelist_expand_trait<TransformationTrait, typelist<Types...>>
    { using type = typename TransformationTrait<Types...>::type; };
    
    template<
        template<typename...> class BinaryAccumulator, 
        typename Init,
        typename... Types
    > struct typelist_accumulate<BinaryAccumulator, Init, typelist<Types...>>
    {
    private:
        template<typename T>
        using accumulator = ::yymp::dtl::typelist_accumulation::accumulator<
            BinaryAccumulator,
            T>;
    public:
        using type = typename decltype(
            (accumulator<Init>{} + ... + accumulator<Types>{})
        )::type;
    };
    
    template<class TypeList, typename T>
        requires is_typelist<TypeList>::value
    struct typelist_unique_accumulator
    {
        using type = typename typelist_append<TypeList, T>::type;
    };
    
    template<typename... Types, typename T>
        requires unique_in_typelist<T, typelist<Types...>>
    struct typelist_unique_accumulator<typelist<Types...>, T>
    {
        using type = typelist<Types...>;
    };
    
    template<typename T, typename... Types>
    struct typelist_all_of<T, typelist<Types...>>
        : ::std::bool_constant<(::std::is_same<T, Types>::value && ...)> { };
    
    template<typename T, typename... Types>
    struct typelist_any_of<T, typelist<Types...>>
        : ::std::bool_constant<(::std::is_same<T, Types>::value || ...)> { };
        
    template<typename T, typename... Types>
    struct typelist_none_of<T, typelist<Types...>>
        : ::std::bool_constant<(!::std::is_same<T, Types>::value && ...)> { };
    
    template<template<typename...> class UnaryTypeTrait, typename... Types>
    struct typelist_all_where<UnaryTypeTrait, typelist<Types...>>
        : ::std::bool_constant<(
            ::std::is_same<
                typename UnaryTypeTrait<Types>::type, 
                ::std::true_type
            >::value && ...
        )> { };
    
    template<template<typename...> class UnaryTypeTrait, typename... Types>
    struct typelist_any_where<UnaryTypeTrait, typelist<Types...>>
        : ::std::bool_constant<(
            ::std::is_same<
                typename UnaryTypeTrait<Types>::type, 
                ::std::true_type
            >::value || ...
        )> { };
    
    template<template<typename...> class UnaryTypeTrait, typename... Types>
    struct typelist_none_where<UnaryTypeTrait, typelist<Types...>>
        : ::std::bool_constant<(
            !::std::is_same<
                typename UnaryTypeTrait<Types>::type, 
                ::std::true_type
            >::value && ...
        )> { };
    
    template<typename T, typename... Types>
    struct typelist_count_of<T, typelist<Types...>>
        : ::std::integral_constant<
            ::std::size_t,
            (::std::size_t{0} + ... 
                + ::std::size_t{::std::is_same<T, Types>::value})
        > { };
    
    template<template<typename...> class UnaryTypeTrait, class TypeList>
        requires is_typelist<TypeList>::value
    struct typelist_count_where
        : typelist_count_of<
            ::std::true_type,
            typename typelist_transform<UnaryTypeTrait, TypeList>::type
        >::type { };
    
    template<typename T, class TypeList>
        requires is_typelist<TypeList>::value
    struct typelist_indices_of
    {
        using type = typename ::yymp::dtl::typelist_indexing::indices_of<
            T,
            TypeList
        >::type;
    };
    
    template<template<typename...> class UnaryTypeTrait, class TypeList>
        requires is_typelist<TypeList>::value
    struct typelist_indices_where
    {
        using type = typename typelist_indices_of<
            ::std::true_type,
            typename typelist_transform<UnaryTypeTrait, TypeList>::type
        >::type;
    };
    
    template<template<typename...> class UnaryTypeTrait, class TypeList>
        requires is_typelist<TypeList>::value
    struct typelist_filter
    {
        using type = typename typelist_select<
            typename typelist_indices_where<UnaryTypeTrait, TypeList>::type,
            TypeList
        >::type;
    };
    
    template<class TypeList>
        requires is_typelist<TypeList>::value
    struct typelist_filter_duplicates
    {
        using type = typename typelist_accumulate<
            typelist_unique_accumulator,
            typelist<>,
            TypeList
        >::type;
    };
    
    template<template<typename...> class TransformationTrait, class TypeList>
        requires is_typelist<TypeList>::value
    struct typelist_group_by
    {
    private:
        using mapped = typename typelist_transform<
            TransformationTrait, 
            TypeList>::type;
    
        template<typename Key>
        using filter_to_key = typename typelist_select<
            typename typelist_indices_of<Key, mapped>::type,
            TypeList
        >::type;
    public:
        using type = typename typelist_transform<
            filter_to_key,
            typename typelist_filter_duplicates<mapped>::type
        >::type;
    };
    
    template<typename... Types, class UnaryFunction>
    constexpr
    UnaryFunction for_each(typelist<Types...>, UnaryFunction f)
    {
        ((f(type_marker<Types>{}), void()), ...);
        return ::std::move(f);
    }
}

#endif // YYMP_TYPELIST_HPP