#ifndef YYMP_UTILITY_HPP
#define YYMP_UTILITY_HPP

#include <type_traits>
#include <utility>

namespace yymp
{
    template<typename T> struct type_marker { using type = T; };
}

#endif // YYMP_UTILITY_HPP