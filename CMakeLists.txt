cmake_minimum_required(VERSION 3.22.1)
project(yymp
    VERSION 1.0.0
    DESCRIPTION "A header-only library of random stuff."
    HOMEPAGE_URL "https://github.com/surrealwaffle/yymp"
)

add_library(${PROJECT_NAME} INTERFACE)

OPTION(YYMP_ENABLE_TESTING "ON to enable testing, OFF to disable" ON)

target_compile_features(
    ${PROJECT_NAME}
    INTERFACE
        cxx_std_20
)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(YYMP_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()