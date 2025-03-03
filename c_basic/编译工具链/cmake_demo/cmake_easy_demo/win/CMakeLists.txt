cmake_minimum_required(VERSION 3.5)

project(boost_asio_sync LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# 每个target 都需要指明的信息: 
# *1.add_dependencies():
# 	 指明target 依赖哪个子项目
# 	 demo: add_dependencies(redis_srv_test hiredis)
# *2.target_include_directories():
# 	 指明target 依赖哪些头文件(linux 可以不添加, 安装依赖库之后, gcc 会自动找到各种头文件存放的地方; windows 需要指定具体路径)
# 	 demo: target_include_directories(redis_srv_test PUBLIC hiredis)
# 	 			 target_include_directories(${CURRENT_LIBRARY} PUBLIC C:/adan_app/boost_1_67_0/boost)
# *3.target_link_libraries():
# 	 指明target 依赖哪些库文件(linux 只需要指定编译选项即可, 安装依赖库之后, 系统会自动把动态链接库, 添加到/usr/lib;
# 													windows 需要指定具体路径, 但windows 不需要指定编译选项, windows 导入库会自动识别API, 根本不需要所谓的编译选项, 编译选项是linux 的专属概念词)
# 	 demo: target_link_libraries(redis_srv_test hiredis)



# 可以整个项目指定的信息:
# *1.add_subdirectory():
# 	 添加git 子项目的路径[git 子项目会被先编译] (不会自动下载git, 这里只做比喻)
# *2.include_directories():
# 	 添加源码目录路径
# *3.find_package():
# 	 查找库路径



# 添加子项目的路径[子项目会被先编译]
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/hiredis)

# 查找标准qt 库
#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
#find_package(Qt ${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# 开启boost cmake 调试选项
#set(Boost_DEBUG ON)
# 设置boost库搜索路径
set(BOOST_ROOT C:/adan_app/boost_1_67_0)
set(BOOST_INCLUDEDIR C:/adan_app/boost_1_67_0/boost)
set(BOOST_LIBRARYDIR C:/adan_app/boost_1_67_0/lib64-msvc-14.1)
# 设置boost 二进制编译平台info
set(Boost_LIB_PREFIX "lib64")
set(Boost_ARCHITECTURE "-msvc")
# 只搜索上语句设置的搜索路径(其他路径不管)
set(Boost_NO_SYSTEM_PATHS ON)
# 查找boost 库(REQUIRED = 找不到就报错, COMPONENTS = 你要找的boost 库, 找到之后, 就可以使用一些boost 专门为cmake 设置的全局变量)
#find_package(Boost 14.1 COMPONENTS system filesystem thread REQUIRED)
# 找不到库, 就不设置具体的库名, 找到boost 就算了
find_package(Boost 1.67.0 REQUIRED)

# 如果找到boost 库, 则打印关键信息, 否则终止继续执行
if(Boost_FOUND)
	MESSAGE(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
	MESSAGE(STATUS "BOOST_LIBRARYDIR = ${BOOST_LIBRARYDIR}")
	MESSAGE(STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}")
else()
	MESSAGE(FATAL_ERROR "cant find_package Boost 1.67.0")
endif()



# 批量添加源码目录[当前目录]
#include_directories( ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src )

# 指定每一个target 需要的文件, 添加到全局资源中(windows 平台特有的步骤)
file(GLOB_RECURSE files_tcp_sync_cli "*.h" "boost_asio_tcp_sync_cli.cpp" )
file(GLOB_RECURSE files_tcp_sync_srv "*.h" "boost_asio_tcp_sync_srv.cpp" )
file(GLOB_RECURSE files_udp_sync_cli "*.h" "boost_asio_udp_sync_cli.cpp" )
file(GLOB_RECURSE files_udp_sync_srv "*.h" "boost_asio_udp_sync_srv.cpp" )



# 添加执行文件target
add_executable(tcp_sync_cli ${files_tcp_sync_cli})
# 添加target 依赖的子项目
#add_dependencies(tcp_sync_cli hiredis)
# 添加头文件目录(PRIVATE = 某个target 私有 / PUBLIC = 所有target 共享)
target_include_directories(tcp_sync_cli PUBLIC ${Boost_INCLUDE_DIRS})
# 链接qt 动态链接库
target_link_libraries(tcp_sync_cli ${BOOST_LIBRARYDIR})
# 连接选项
#target_link_options(tcp_sync_cli PRIVATE /machine:x64)



add_executable(tcp_sync_srv ${files_tcp_sync_srv})
target_include_directories(tcp_sync_srv PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(tcp_sync_srv ${BOOST_LIBRARYDIR})
#target_link_options(tcp_sync_srv PRIVATE /machine:x64)

add_executable(udp_sync_cli ${files_udp_sync_cli})
target_include_directories(udp_sync_cli PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(udp_sync_cli ${BOOST_LIBRARYDIR})
#target_link_options(udp_sync_cli PRIVATE /machine:x64)

add_executable(udp_sync_srv ${files_udp_sync_srv})
target_include_directories(udp_sync_srv PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(udp_sync_srv ${BOOST_LIBRARYDIR})
#target_link_options(udp_sync_srv PRIVATE /machine:x64)



# 包含gnu 头
include(GNUInstallDirs)



# 设置安装路径
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)
# 设置安装细节
install(TARGETS tcp_sync_cli tcp_sync_srv udp_sync_cli udp_sync_srv
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

