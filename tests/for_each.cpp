//          Copyright Thomas Goehner 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <cstdint>
#include <utility>

#include <yymp/typelist_fwd.hpp>
#include <yymp/for_each.hpp>

using yymp::typelist;
using yymp::type_value;

struct sum_size {
    template< class T >
    constexpr void operator()(yymp::type_value<T>) noexcept {
        value += sizeof(T);
    }
    
    constexpr void operator()(std::size_t i) noexcept {
        value += i;
    }
    std::size_t value;
};

struct shift_and_sum_size {
    static constexpr std::size_t MAX_SIZE = 8u;
    
    template< class T >
    constexpr void operator()(yymp::type_value<T>) noexcept {        
        static_assert(sizeof(T) <= MAX_SIZE, "require size to fit in MAX_SIZE");
        value = value * MAX_SIZE + sizeof(T);
    }
    
    constexpr void operator()(std::size_t i) noexcept {
        value = value * MAX_SIZE + i;
    } 
    
    std::size_t value;
};

using test_typelist = typelist<std::int8_t, std::int16_t, std::int64_t>;
using size_iseq = std::integer_sequence< std::size_t, sizeof(std::int8_t), sizeof(std::int16_t), sizeof(std::int64_t) >;

static_assert(yymp::for_each(test_typelist{}, sum_size{0}).value == 11u, "");               // test (no order)
static_assert(yymp::for_each(test_typelist{}, shift_and_sum_size{0}).value ==  88u, "");    // test (order)

static_assert(yymp::for_each(size_iseq{}, sum_size{0}).value == 11u, "");               // test (no order)
static_assert(yymp::for_each(size_iseq{}, shift_and_sum_size{0}).value ==  88u, "");    // test (order)