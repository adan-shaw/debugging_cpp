#
# 编译前的环境检查
# 	检查cmake 版本
# 	检查系统版本OS
# 	检查编译器版本
# 	检查依赖库/依赖软件是否安装且版本是否正确(glibc,openssl-dev 等等)



# 显示 cmake version
MESSAGE("* cmake version: ${CMAKE_MINIMUM_REQUIRED}")
# 主版本号, 3
MESSAGE("* cmake version: ${CMAKE_MAJOR_VERSION}")
# 次版本好, 0
MESSAGE("* cmake version: ${CMAKE_MINOR_VERSION}")
# 补丁版本好, 0
MESSAGE("* cmake version: ${CMAKE_PATCH_VERSION}")

# 显示'系统环境'变量: $PATH (使用 $ENV{NAME} 指令就可以显示'系统的环境变量'的字符串了)
MESSAGE("PATH 路径: $ENV{PATH}")

# 检查操作系统info
IF(WIN32)
	MESSAGE(FATAL_ERROR "STOP!! unix only, this system is: ${CMAKE_SYSTEM}")
ELSEIF(UNIX)
	# unix-like only
	MESSAGE("* 显示系统类型: ${CMAKE_HOST_SYSTEM_NAME}")
	MESSAGE("* CMAKE_HOST_UNIX: ${CMAKE_HOST_UNIX}")
	MESSAGE("* CMAKE_HOST_WIN32: ${CMAKE_HOST_WIN32}")
ELSE()
	MESSAGE(FATAL_ERROR "STOP!! unknow system type, this system is: ${CMAKE_SYSTEM}")
ENDIF(WIN32)



# 指定交叉编译链
#INCLUDE(./crosstools.cmake)



# 检查是否是intel x86架构的芯片, 是就生成X86 = TRUE 宏
IF(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)") 
	SET(X86 TRUE)
ELSE()
	SET(X86 FALSE)
ENDIF()
# 检查是否是arm 架构的芯片, 是就生成ARM = TRUE 宏
IF(CMAKE_SYSTEM_PROCESSOR MATCHES "(arm)|(ARM)|(aarch64)|(AARCH64)") 
	SET(ARM TRUE)
ELSE()
	SET(ARM FALSE)
ENDIF()



# 检查是否装载c 编译器
IF(CMAKE_C_COMPILER_LOADED)
	MESSAGE("* CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")
	MESSAGE("* CMAKE_C_COMPILER_VERSION: ${CMAKE_C_COMPILER_VERSION}")
ELSE()
	MESSAGE("* c compiler is not loaded ")
	#MESSAGE(FATAL_ERROR "STOP!! c compiler is not loaded;")
ENDIF()
# 检查是否装载c++ 编译器
IF(CMAKE_CXX_COMPILER_LOADED)
	MESSAGE("* CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
	MESSAGE("* CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
ELSE()
	MESSAGE("* c++ compiler is not loaded ")
	#MESSAGE(FATAL_ERROR "STOP!! c++ compiler is not loaded;")
ENDIF()
# 当前使用的是否是交叉编译链
MESSAGE("* CMAKE_CROSSCOMPILING: ${CMAKE_CROSSCOMPILING}")



IF(CMAKE_CXX_COMPILER_LOADED)
	# 包含/usr/share/cmake-3.13/Modules/CheckCXXSourceCompiles.cmake
	INCLUDE(CheckCXXCompilerFlag)

	# 检查gcc 支持的c++ 版本
	CHECK_CXX_COMPILER_FLAG(-std=c++2a SUPPORT_CPP20)
	CHECK_CXX_COMPILER_FLAG(-std=c++11 SUPPORT_CPP11)
	CHECK_CXX_COMPILER_FLAG(-std=c++0x SUPPORT_CPP0X)
	IF(SUPPORT_CPP20)
		# c++20
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
		MESSAGE("* using flag -std=c++2a ")
	ELSEIF(SUPPORT_CPP11)
		# c++11
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
		MESSAGE("* using flag -std=c++11 ")
	ELSEIF(SUPPORT_CPP0X)
		# c++03
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
		MESSAGE("* using flag -std=c++0x ")
	ELSE()
		MESSAGE(FATAL_ERROR "unknow error: the compiler c++ version problems: ${CMAKE_CXX_COMPILER}")
	ENDIF()
ENDIF()



# 检查依赖(忽略)
