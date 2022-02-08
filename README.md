# yymp
*yymp* is a library of some miscellaneous items I've created:
 * [`yymp::typelist`](include/yymp/typelist.hpp) and accompanying templates for manipulation;
 * [`yymp::wref_tuple`](include/yymp/wref_tuple.hpp), a tuple for providing a flat view of multiple tuple references;
 * [`yymp::stuple`](include/yymp/stuple.hpp), an aggregate tuple with a focus on improved compilation times.

# Requirements
 * A C++ compiler supporting C++20

# Example Usage (typelist)

To include all the `typelist` facilities, simply `#include <yymp/typelist.hpp>`.
To create a `typelist`, one writes
```cpp
using my_types = yymp::typelist<int, char*, int, void*>;
```

Sometimes, it may be necessary to join two or more typelists together:
```cpp
using foo_types = yymp::typelist<void, void, short>;
using my_joined_types = yymp::typelist_join_t<foo_types, my_types, foo_types>;
// -> yymp::typelist<void, void, short, int, char*, int, void*, void, void, short>;
```

Other times, we may have a `typelist` of `typelist`s that we need to combine:
```cpp
using recombined_typelist = yymp::typelist_expand_trait_t<
    yymp::typelist_join,
    TypeListOfTypeLists
>;
```

One can also group types by a `TransformationTrait`, where the key for a type 
`T` is given by `TransformationTrait<T>::type`. As `UnaryTypeTrait`s 
satisfy this requirement, that means the following is possible:
```cpp
using my_groups = yymp::typelist_group_by_t<std::is_pointer, my_types>; 
// -> yymp::typelist<yymp::typelist<int, int>, yymp::typelist<char*, void*>>
```
The order in which the groups appear is by first occurrence.

If instead you want only the types which are pointers, then one can use `yymp::filter`:
```cpp
using filtered_types = yymp::typelist_filter_t<std::is_pointer, my_types>;
// -> yymp::typelist<char*, void*>;
```

Or perhaps you need to ensure there are no duplicates:
```cpp
using no_duplicates = yymp::typelist_filter_duplicates_t<my_joined_types>;
// -> yymp::typelist<void, short, int, char*, void*>;
```

See [`yymp/typelist.hpp`](include/yymp/typelist.hpp) for more.

The following concepts may also be of interest, even if you
are not dealing with typelists directly:
```cpp
// Passes if there is EXACTLY one occurrence of T among Types
template<typename T, typename... Types>
concept unique_among = ...;

// Passes if there is AT LEAST one occurrence of T among Types
template<typename T, typename... Types>
concept any_among = ...;
```

# stuple

To see the difference in speed between `std::tuple` and `yymp::stuple`, see the 
[`stuple` stress test preamble](tests/stuple_stress.cpp). Your results may 
differ, depending on the implementation's `std::tuple`.

# License
*yymp* is licensed under the [Boost Software License](https://github.com/surrealwaffle/yymp/blob/master/LICENSE_1_0.txt).
