cmake_minimum_required(VERSION 3.12)

project(miass)

if (MSVC)
    set(miass_compile_options /W4)
else ()
    set(miass_compile_options -Wall -Wextra -pedantic)
endif ()

file(GLOB_RECURSE miass_sources src/*.cpp include/*.hpp)

add_executable(miass ${miass_sources})
target_include_directories(miass PRIVATE include)
target_compile_features(miass PRIVATE cxx_std_17)
target_compile_options(miass PRIVATE ${miass_compile_options})
