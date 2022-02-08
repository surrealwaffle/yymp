cmake_minimum_required(VERSION 3.22.1)
project(yymp_testing)

add_executable(yymp_typelist_tests typelist_basic.cpp)
target_link_libraries(yymp_typelist_tests PRIVATE yymp)

add_executable(yymp_stuple_tests stuple_basic.cpp)
target_link_libraries(yymp_stuple_tests PRIVATE yymp)

add_test(NAME yymp_typelist_basic_tests COMMAND yymp_typelist_tests)
add_test(NAME yymp_stuple_basic_tests COMMAND yymp_stuple_tests)

if(YYMP_STUPLE_STRESS_TEST)
    add_executable(yymp_stuple_stress_test0 stuple_stress.cpp)
    target_link_libraries(yymp_stuple_stress_test0 PRIVATE yymp)
    add_test(NAME yymp_stuple_stress_test0 COMMAND yymp_stuple_stress_test0)
endif()