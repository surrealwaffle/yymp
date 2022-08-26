cmake_minimum_required(VERSION 3.24.1)
project(yymp
    VERSION 2.0.0
    DESCRIPTION "A header-only library of random stuff."
    HOMEPAGE_URL "https://github.com/surrealwaffle/yymp"
    LANGUAGES CXX
)

add_library(yymp_yymp INTERFACE)
add_library(yymp::yymp ALIAS yymp_yymp)

set_target_properties(yymp_yymp
    PROPERTIES
        EXPORT_NAME yymp
)

target_compile_features(yymp_yymp
    INTERFACE
        cxx_std_20
)

target_include_directories(yymp_yymp
    INTERFACE
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

include(CTest)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()