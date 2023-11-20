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
# 	 			 target_include_directories(${CURRENT_LIBRARY} PUBLIC D:/adan_app_home/boost_1_67_0/boost)
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
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# 批量添加源码目录[当前目录]
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/src
)



# 添加执行文件target
add_executable(tcp_sync_cli 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/boost_asioEx.h 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/boost_asio_tcp_sync_cli.cpp
)
# 添加target 依赖的子项目
#add_dependencies(tcp_sync_cli hiredis)
# 添加头文件目录(PRIVATE = 某个target 私有 / PUBLIC = 所有target 共享)
#target_include_directories(tcp_sync_cli PUBLIC D:/adan_app_home/boost_1_67_0/boost)
# 链接qt 动态链接库
target_link_libraries(tcp_sync_cli pthread boost_system)



add_executable(tcp_sync_srv 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/boost_asioEx.h 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/boost_asio_tcp_sync_srv.cpp
)
# linux 平台不需要指定头文件搜索路径(默认安装在/usr/include, gcc默认就找到该头文件, 不用再指明)
#target_include_directories(tcp_sync_srv PUBLIC D:/adan_app_home/boost_1_67_0/boost)
# linux 平台只需要指定编译选项即可(默认安装在/usr/lib, gcc默认就找到该库文件, 添加编译选项, 告诉gcc 使用了哪个库即可)
target_link_libraries(tcp_sync_srv pthread boost_system)

add_executable(udp_sync_cli 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/boost_asioEx.h 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/boost_asio_udp_sync_cli.cpp
)
#target_include_directories(udp_sync_cli PUBLIC D:/adan_app_home/boost_1_67_0/boost)
target_link_libraries(udp_sync_cli pthread boost_system)

add_executable(udp_sync_srv 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/boost_asioEx.h 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/boost_asio_udp_sync_srv.cpp
)
#target_include_directories(udp_sync_srv PUBLIC D:/adan_app_home/boost_1_67_0/boost)
target_link_libraries(udp_sync_srv pthread boost_system)



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

