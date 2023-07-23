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
PROJECT(${target_name})

# 显示项目主路径(即主CMakeLists.txt 所在地)
MESSAGE("* HOME = ${PROJECT_SOURCE_DIR}")

# 默认设置为debug 版本
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



# 包含*.cmake, 用INCLUDE()
# 包含子文件夹中的CMakeLists.txt, 用INCLUDE_DIRECTORIES() [一个文件夹, 只有一个CMakeLists.txt]

# 本层CMakeLists.txt 的结构描述体(描述本层目录的文件&文件夹详情)
INCLUDE_DIRECTORIES(
	.
)

# 添加包含'二级CMakeLists.txt'的子目录
ADD_SUBDIRECTORY(dynamic_lib)
ADD_SUBDIRECTORY(static_lib)
ADD_SUBDIRECTORY(src1)
ADD_SUBDIRECTORY(src2)



# 当前目录下所有源文件赋给DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

# 变量指明, 所有的头文件
SET(SRC "../*.cpp")

# 变量指明, 所有的源文件
SET(HEAD "../*.h")

# 变量指明,所有的.o文件
SET(TMP_O "../*.o")



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

# 指定库文件路径
LINK_DIRECTORIES(
	"./static_lib/"
	"./dynamic_lib/"
)

# 添加生成目标='共享链接库', 并自动编译生成: 
# target=静态.a
ADD_LIBRARY("./static_lib/static_lib.a" STATIC "./static_lib/static_lib.cpp")
SET_TARGET_PROPERTIES(PROPERTIES OUTPUT_NAME "static_lib")
SET_TARGET_PROPERTIES(PROPERTIES VERSION 1.2 SOVERSION 1)

ADD_LIBRARY("./static_lib/static_lib2.a" STATIC "./static_lib/static_lib2.cpp")
SET_TARGET_PROPERTIES(PROPERTIES OUTPUT_NAME "static_lib2")
SET_TARGET_PROPERTIES(PROPERTIES VERSION 1.2 SOVERSION 1)

# target=动态.so
ADD_LIBRARY("./dynamic_lib/libdynamic.so" SHARED "./dynamic_lib/libdynamic.cpp")
SET_TARGET_PROPERTIES(PROPERTIES OUTPUT_NAME "libdynamic")
SET_TARGET_PROPERTIES(PROPERTIES VERSION 1.2 SOVERSION 1)



# 指认生成的target .exe 文件所需的.a/.so 链接库.
TARGET_LINK_LIBRARIES(${target_name} static_lib.a)
TARGET_LINK_LIBRARIES(${target_name} static_lib2.a)
TARGET_LINK_LIBRARIES(${target_name} libdynamic.so)

# 添加生产目标='.o 半成品-可执行程序', 并自动编译生成:
# target=.o
ADD_EXECUTABLE(${target_name} ${DIR_SRCS})

# 执行编译各个.a 子静态库
#ADD_EXECUTABLE("g++ -c static_lib2.cpp")
#ADD_EXECUTABLE("g++ -c static_lib2.cpp")
#ADD_EXECUTABLE("ar cr static_lib.a static_lib.o")
#ADD_EXECUTABLE("ar cr static_lib2.a static_lib2.o")

########################################################



# 生成Makefile后, 运行make test 来执行测试
ADD_TEST(test1 ${target_name} "fuck you 参数")
ADD_TEST(test2 ${target_name} "fuck you 参数2")
# 如果没有在'同一个'CMakeLists.txt中, 打开ENABLE_TESTING()指令,
# 任何ADD_TEST()都是无效的.

# 打开make test 开关
ENABLE_TESTING()





