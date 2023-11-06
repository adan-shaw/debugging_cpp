cmake_minimum_required(VERSION 3.14)

project(make_dll_easy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(make_dll_easy SHARED
  make_dll_easy.cpp
  make_dll_easy.h
)

target_compile_definitions(make_dll_easy PRIVATE MAKE_DLL_EASY_LIBRARY)
