cmake_minimum_required(VERSION 3.11)

project(x)

set(CMAKE_CXX_STANDARD 11)

add_executable(x main.cpp)

# 链接静态链接库/usr/lib/x86_64-linux-gnu/libz.a (两种方法, 只能用一种)
#target_link_libraries (${PROJECT_NAME} libz.a)

# 链接动态链接库/usr/lib/x86_64-linux-gnu/libz.so
target_link_libraries (${PROJECT_NAME} z)

