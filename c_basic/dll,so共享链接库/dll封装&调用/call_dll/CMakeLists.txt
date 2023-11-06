cmake_minimum_required(VERSION 3.5)

project(call_dll LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(call_dll
	call_dll.cpp call_dll.h)

include(GNUInstallDirs)
install(TARGETS call_dll
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
