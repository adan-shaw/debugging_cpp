#
# 本cmake function, 一般用作指定编译工具链的位置
#



# Indicate cross-compilation     [必需]开启交叉编译
SET(CMAKE_CROSSCOMPILING true)
#SET(CMAKE_CROSSCOMPILING false)

# Indicate target operate system [必需]编译的目标平台
SET(CMAKE_SYSTEM_NAME Linux)
#SET(CMAKE_SYSTEM_NAME Generic)

# Indicate architecture          [必需]目标系统的芯片架构体系
#SET(CMAKE_SYSTEM_PROCESSOR arm)
#SET(CMAKE_SYSTEM_PROCESSOR aarch64)
#SET(CMAKE_SYSTEM_PROCESSOR x86)
SET(CMAKE_SYSTEM_PROCESSOR x86_64)



# Indicate toolchain path        [自定义]编译工具链的home路径
SET(TOOLCHAIN_HOME "/usr")

# The first search path          [必需]指定'交叉编译环境安装目录 & 交叉编译环境的目录'(一般是编译工具链的home路径)
SET(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_HOME})

# The second search path         [非必须]指定编译器--sysroot编译参数(CMAKE_SYSROOT只有在3.0以上的版本才有效)
# 默认先搜索CMAKE_FIND_ROOT_PATH, 之后搜索CMAKE_SYSROOT, 最后搜索其他non-rooted路径;
# 默认路径: /usr/x86_64-linux-gnu (很诡异, 这个路径下只有include)
SET(CMAKE_SYSROOT ${TOOLCHAIN_HOME}/x86_64-linux-gnu)

# C/C++ compiler path            [必需]编译器bin 路径
#SET(CMAKE_C_COMPILER ${TOOLCHAIN_HOME}/bin/x86_64-linux-gnu-gcc)
#SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_HOME}/bin/x86_64-linux-gnu-g++)

SET(CMAKE_AR "${TOOLCHAIN_HOME}/bin/ar")
SET(CMAKE_C_COMPILER_AR "${TOOLCHAIN_HOME}/bin/gcc-ar")
SET(CMAKE_CXX_COMPILER_AR "${TOOLCHAIN_HOME}/bin/gcc-ar")

SET(CMAKE_RANLIB "${TOOLCHAIN_HOME}/bin/ranlib")
SET(CMAKE_C_COMPILER_RANLIB "${TOOLCHAIN_HOME}/bin/gcc-ranlib")
SET(CMAKE_CXX_COMPILER_RANLIB "${TOOLCHAIN_HOME}/bin/gcc-ranlib")

SET(CMAKE_LINKER "${TOOLCHAIN_HOME}/bin/ld")



# The install path               [非必须]cmake执行install目标时默认路径
#SET(CMAKE_INSTALL_PREFIX /home/adan/)
SET(CMAKE_INSTALL_PREFIX "./build")



# 从来不在指定目录下查找工具程序(在大多数情况下, 需要构建主机的可执行文件, 因此通常将CMAKE_FIND_ROOT_PATH_MODE_PROGRAM设置为NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# 只在指定目录下查找库文件(在大多数情况下, 这用于查找将用于链接的库(需要用于目标的库), 因此通常将其设置为ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)

# 只在指定目录下查找头文件(在许多情况下, 这用于查找包含目录(应搜索目标环境), 因此通常将其设置为ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)



MESSAGE("* ++++++++++++++++++++++++++++++++++++++++++++++++")
MESSAGE("* crosstools.cmake loaded, info:")
MESSAGE("* ++++++++++++++++++++++++++++++++++++++++++++++++")
MESSAGE("* CMAKE_CROSSCOMPILING=${CMAKE_CROSSCOMPILING} ")
MESSAGE("* CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME} ")
MESSAGE("* CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR} ")
MESSAGE("* TOOLCHAIN_HOME=${TOOLCHAIN_HOME} ")
MESSAGE("* CMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH} ")
MESSAGE("* CMAKE_SYSROOT=${CMAKE_SYSROOT} ")
MESSAGE("* CMAKE_C_COMPILER=${CMAKE_C_COMPILER} ")
MESSAGE("* CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} ")
MESSAGE("* CMAKE_AR=${CMAKE_AR} ")
MESSAGE("* CMAKE_C_COMPILER_AR=${CMAKE_C_COMPILER_AR} ")
MESSAGE("* CMAKE_CXX_COMPILER_AR=${CMAKE_CXX_COMPILER_AR} ")
MESSAGE("* CMAKE_RANLIB=${CMAKE_RANLIB} ")
MESSAGE("* CMAKE_C_COMPILER_RANLIB=${CMAKE_C_COMPILER_RANLIB} ")
MESSAGE("* CMAKE_CXX_COMPILER_RANLIB=${CMAKE_CXX_COMPILER_RANLIB} ")
MESSAGE("* CMAKE_LINKER=${CMAKE_LINKER} ")
MESSAGE("* CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} ")
MESSAGE("* ++++++++++++++++++++++++++++++++++++++++++++++++")

