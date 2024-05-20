cmake_minimum_required(VERSION 3.24.1)
project(yymp
    VERSION 2.0.0
    DESCRIPTION "A header-only library of random stuff."
    HOMEPAGE_URL "https://github.com/surrealwaffle/yymp"
    LANGUAGES CXX
)

option(YYMP_BUILD_TESTS
    "Build tests for rwc::transport [default=OFF]"
)

add_library(yymp_yymp INTERFACE)
add_library(yymp::yymp ALIAS yymp_yymp)

target_compile_features(yymp_yymp
    INTERFACE
        cxx_std_20
)

target_include_directories(yymp_yymp
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

include(CTest)
if(BUILD_TESTING AND YYMP_BUILD_TESTS)
    add_subdirectory(tests)
endif()