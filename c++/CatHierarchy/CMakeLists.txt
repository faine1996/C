cmake_minimum_required(VERSION 3.10)
project(CatHierarchy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CatHierarchy
    src/main.cpp
    src/Cat.cpp
    src/StreetCat.cpp
    src/SiamiCat.cpp
    src/SiamiStreetCat.cpp
)

target_include_directories(CatHierarchy PRIVATE inc)