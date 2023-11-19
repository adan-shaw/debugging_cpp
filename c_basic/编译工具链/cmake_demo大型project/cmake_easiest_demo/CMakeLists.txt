cmake_minimum_required(VERSION 3.8)

project(boost_sync LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找标准qt 库
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# 添加执行文件
add_executable(boost_sync
  main.cpp
)

# 链接qt 库
target_link_libraries(boost_sync Qt${QT_VERSION_MAJOR}::Core)

# 包含gnu 头
include(GNUInstallDirs)

# target 安装细节
install(TARGETS boost_sync
		BUNDLE DESTINATION .
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
