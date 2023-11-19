# 普通打印
#MESSAGE("* cmake version: ${CMAKE_PATCH_VERSION}")

# 输出到<STATUS>频道: 打印进度字符串, 会自动添加'--'(非全功能打印, 不能打印宏, 最好别用)
#MESSAGE(STATUS "cmake version: ${CMAKE_PATCH_VERSION}")

# 输出到<FATAL_ERROR>频道: 打印字符串, 并终止cmake
#MESSAGE(FATAL_ERROR "stop MESSAGE")



# 声明cmake 最小版本, 需要3.13以上(一般本机是3.13.4, 则cmake 检查以3.13 为最小, 这是默认规则, 容大不容小)
CMAKE_MINIMUM_REQUIRED(VERSION 3.13 FATAL_ERROR)

# 开启if else 语法支持
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)



# 指定交叉编译链(注: 这两条命令应该放在文件的开始位置(cmake_minimum_required命令之下, 在其他命令之上, 否则可能无效)
INCLUDE(./crosstools.cmake)

# 包含自动检查模块(自动检查模块会自动继承已经指定的交叉编译链,
# 							或者你可以在auto_check.cmake 中包含crosstools.cmake, 强制指定交叉编译链)
INCLUDE(./auto_check.cmake)



# 获得当前所在文件夹的名字(废弃)
GET_FILENAME_COMPONENT(FLODER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 设置生成目标'可执行exe文件'的名
SET(target_name "obj_out")
PROJECT(
	${target_name}
	LANGUAGES CXX
	VERSION "1.0.0"
)

# 指定c++ 版本
set(CMAKE_CXX_STANDARD 20)
# C++11 C++14 C++17 C++20
set(CMAKE_CXX_FLAGS "-std=c++20")
MESSAGE("* CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} ")

# 显示项目主路径(即主CMakeLists.txt 所在地)
MESSAGE("* HOME = ${PROJECT_SOURCE_DIR}")

# 如果没有设置编译版本, 则默认设置为debug 版本
IF(NOT CMAKE_BUILD_TYPE)
	#SET(CMAKE_BUILD_TYPE Release)
	SET(CMAKE_BUILD_TYPE Debug)
ENDIF()



#
# 指导cmake如何生成makefile
#
# 	声明编译过程所需的变量
# 	添加编译宏
# 	添加编译选项
# 	添加第三方源码库
# 	添加第三方头文件库
# 	添加第三方头链接库(.so / .a)
# 	第三方路径汇总
# 	添加链接选项
# 	添加编译target
#



# 强制使用静态库生成exe 可执行文件, 完全不用.so 动态链接库?
#SET(BUILD_SHARED_LIBS ON)



#CMAKE_C_FLAGS					= CFLAGS   = c编译选项
#CMAKE_CXX_FLAGS				= CXXFLAGS = c++编译选项
#CMAKE_EXE_LINKER_FLAGS = LDFLAGS  = 链接库选项
SET(CMAKE_C_FLAGS "-O2")
SET(CMAKE_CXX_FLAGS "-O2")
SET(CMAKE_EXE_LINKER_FLAGS "-lpthread")

# 强制使用gnu 标准去编译src源文件
ADD_DEFINITIONS(-D_GNU_SOURCE)

# 根据CMAKE_BUILD_TYPE, 选取编译选项
IF(CMAKE_BUILD_TYPE MATCHES "Debug")
	MESSAGE("* CMAKE_BUILD_TYPE = Debug !!")
	SET(CMAKE_CXX_FLAGS_All "-Wall -g3")
ELSEIF(CMAKE_BUILD_TYPE MATCHES "Release")
	MESSAGE("* CMAKE_BUILD_TYPE = Release !!")
	SET(CMAKE_CXX_FLAGS_RELEASE "-Wall")
	# 指定禁用assert()
	ADD_DEFINITIONS(-DNDEBUG)
	SET(CMAKE_C_FLAGS "-O2")
	SET(CMAKE_CXX_FLAGS "-O2")
ELSE()
	MESSAGE("* unknow CMAKE_BUILD_TYPE !!")
	SET(CMAKE_CXX_FLAGS_RELEASE "-Wall")
	# 指定禁用assert()
	ADD_DEFINITIONS(-DNDEBUG)
	SET(CMAKE_C_FLAGS "-O2")
	SET(CMAKE_CXX_FLAGS "-O2")
ENDIF()



########################################################

# 设置使用文件夹属性
set_property(GLOBAL PROPERTY USE_FOLDERS ON)



# 包含*.cmake, 用INCLUDE()
# 包含子文件夹中的CMakeLists.txt, 用INCLUDE_DIRECTORIES() [一个文件夹, 只有一个CMakeLists.txt]

# 找头文件
INCLUDE_DIRECTORIES(
	.
)

# 找库文件( 只找现成的, 可以用: ${PROJECT_SOURCE_DIR} )
LINK_DIRECTORIES(
	"./static_lib"
	"./dynamic_lib"
)



# 添加子CMakeLists.txt(如果'子CMakeLists.txt'中, 有INCLUDE_DIRECTORIES()找头文件, 则等同于包含头文件)
ADD_SUBDIRECTORY(dynamic_lib)
ADD_SUBDIRECTORY(static_lib)

# 当前目录下所有源文件赋给DIR_SRCS(简单项目, 会用)
#AUX_SOURCE_DIRECTORY(. DIR_SRCS)

# 自定义搜索: 源文件(复杂项目会用)
file(GLOB static_lib_h ./static_lib/headfile/*.h)
file(GLOB static_lib_src ./static_lib/*.cpp)
file(GLOB dynamic_lib_h ./dynamic_lib/headfile/*.h)
file(GLOB dynamic_lib_src ./dynamic_lib/*.cpp)

# for test only
#MESSAGE("* headfile: ${static_lib_h}")
#MESSAGE("* src_file: ${static_lib_src}")

# 添加生成目标='共享链接库', 并自动编译生成: 
# target=静态.a [.a 静态链接库不需要链接, 就可以生成]
ADD_LIBRARY("_static" STATIC ${static_lib_src} ${static_lib_h} )
SET_TARGET_PROPERTIES(PROPERTIES OUTPUT_NAME "lib_static.a")
SET_TARGET_PROPERTIES(PROPERTIES VERSION 1.0 SOVERSION 1)

ADD_LIBRARY("_static2" STATIC ${static_lib_src} ${static_lib_h} )
SET_TARGET_PROPERTIES(PROPERTIES OUTPUT_NAME "lib_static2.a")
SET_TARGET_PROPERTIES(PROPERTIES VERSION 1.2 SOVERSION 1)

# target=动态.so (必须要链接, 才能生成.so 共享链接库)
ADD_LIBRARY("_dynamic" SHARED ${dynamic_lib_src} ${dynamic_lib_h} )
SET_TARGET_PROPERTIES(PROPERTIES OUTPUT_NAME "lib_dynamic.so")
SET_TARGET_PROPERTIES(PROPERTIES VERSION 1.1 SOVERSION 1)

# 查找./dynamic_lib/lib_static.a 文件是否存在
FIND_FILE(isExist_static_lib lib_static.a ./static_lib/)
IF(NOT isExist_static_lib)
	MESSAGE(FATAL_ERROR "./static_lib/lib_static.a not found")
ENDIF()

# 查找./dynamic_lib/lib_static2.a 文件是否存在
FIND_FILE(isExist_static_lib2 lib_static2.a ./static_lib/)
IF(NOT isExist_static_lib2)
	MESSAGE(FATAL_ERROR "./static_lib/lib_static2.a not found")
ENDIF()

# 指认生成的library 文件所需的.a/.so 共享链接库(会自动根据命名规则libxxx.so 来获取-l 编译选项, 不用操心)
# (实际上lib_dynamic.so 不需要任何依赖库, 只是展示LINK_LIBRARIES() 的用途)
LINK_LIBRARIES(
	lib_dynamic.so
	${PROJECT_SOURCE_DIR}/build/lib_static.a
	${PROJECT_SOURCE_DIR}/build/lib_static2.a
)



# 添加包含'二级CMakeLists.txt'的子目录
ADD_SUBDIRECTORY(src1)
ADD_SUBDIRECTORY(src2)

# 自定义搜索: 源文件(复杂项目会用)
file(GLOB Srcs ./*.cpp ./src1/*.cpp ./src2/*.cpp )

# 自定义搜索: 头文件(复杂项目会用)
file(GLOB Incs ./*.h ./dynamic_lib/headfile/*.h ./src1/*.h ./src2/*.h ./static_lib/headfile/*.h)

# add resource (qt 项目可能会用)
#set(RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/SiltSLAM.qrc)
#qt5_add_resources(RESOURCE_ADDED ${RESOURCE})

# 添加生产目标='.o 半成品-可执行程序', 并自动编译生成:
# target=.o
ADD_EXECUTABLE(${target_name} ${Srcs} ${Incs})

# 查找./dynamic_lib/lib_dynamic.so 文件是否存在
FIND_FILE(isExist_lib_dynamic lib_dynamic.so ./dynamic_lib/)
IF(NOT isExist_lib_dynamic)
	MESSAGE(FATAL_ERROR "./dynamic_lib/lib_dynamic.so not found")
ENDIF()

# 手动添加编译选项-l
set_target_properties(
	${target_name}
	PROPERTIES
	#LINK_OPTIONS "-l_static -l_static2 -l_dynamic"
	LINK_FLAGS "-l_static -l_static2 -l_dynamic"
)

# 指认生成的target .exe 文件所需的.so 共享链接库
TARGET_LINK_LIBRARIES(
	${target_name}
	${PROJECT_SOURCE_DIR}/static_lib/lib_static.a
	${PROJECT_SOURCE_DIR}/static_lib/lib_static2.a
	${PROJECT_SOURCE_DIR}/dynamic_lib/lib_dynamic.so
)

########################################################





# 生成Makefile后, 运行make test 来执行测试
ADD_TEST(test1 ${target_name} "love you 参数")
ADD_TEST(test2 ${target_name} "love you 参数2")

# 打开make test 开关(如果没有在'同一个'CMakeLists.txt中, 打开ENABLE_TESTING()指令, 则任何ADD_TEST()都是无效的)
ENABLE_TESTING()





