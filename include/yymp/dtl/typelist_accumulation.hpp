// SPDX-License-Identifier: BSL-1.0

#ifndef YYMP_TYPELIST_ACCUMULATION
#define YYMP_TYPELIST_ACCUMULATION

namespace yymp
{
    template<typename... Types>
    struct typelist;
}

namespace yymp::dtl::typelist_accumulation
{
    template<template<class...> class TransformationTrait, typename T>
    struct accumulator { using type = T; };
    
    template<
        template<class...> class TransformationTrait, 
        typename L, typename R
    > constexpr auto operator+(
        const accumulator<TransformationTrait, L>&, 
        const accumulator<TransformationTrait, R>&) noexcept
        -> accumulator<
            TransformationTrait, typename TransformationTrait<L, R>::type
        >
    { return {}; }
}

#endif // YYMP_TYPELIST_ACCUMULATION