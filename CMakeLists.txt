cmake_minimum_required(VERSION 3.16)
project(simplelang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(simplelang
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/optimizer.cpp
    src/codegen.cpp
    src/vm.cpp
)

target_include_directories(simplelang PRIVATE src)
