cmake_minimum_required(VERSION 3.20)

project(Lab_1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(add_exe target path)

    add_executable(${target} ${path})

    target_include_directories(${target} PRIVATE include)

    target_compile_options(${target} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
    )

endfunction()

add_exe(lab1 src/main.cpp)
