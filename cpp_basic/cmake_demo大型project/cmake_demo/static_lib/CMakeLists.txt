# 简单一个MESSAGE()语句, 只是展示如何进行'子CMakeLists.txt'包含操作而已
MESSAGE(STATUS "hello, i'm static_lib '子CMakeLists.txt'")

#
# 执行编译各个.so 子共享库('子CMakeLists.txt'同样不能执行'预先编译'的工作!!)
#
# 在CMakeLists.txt 中, ADD_EXECUTABLE() 不能用来执行一些未知的shell命令!
# 这样是不被允许的, 这样容易产生恶意程序, 系统也不会允许的.
# CMakeLists.txt,cmake中的ADD_EXECUTABLE()一般只有查询权限,可能没有'执行权限x'
#
# 执行编译各个.a 子静态库
#ADD_EXECUTABLE("g++ -c static_lib2.cpp")
#ADD_EXECUTABLE("g++ -c static_lib2.cpp")
#ADD_EXECUTABLE("ar cr static_lib.a static_lib.o")
#ADD_EXECUTABLE("ar cr static_lib2.a static_lib2.o")



#
# 描述二级project 目录关系, 是允许的
#

# 本层CMakeLists.txt 文件结构描述体
# (描述目录, 但目录中不能包含'二级CMakeLists.txt',只能有源文件+头文件)
INCLUDE_DIRECTORIES(
.
)
# 故意不描述no_CMakeLists.txt_test(但仍然正确可行, 证明理论正确!!)

# 二级'子CMakeLists.txt', 不再需要三级add_subdirectory()
# 如果还要调用add_subdirectory(), 则需要三级'孙CMakeLists.txt', 太复杂了!!
#add_subdirectory()



# 当前目录下所有源文件赋给DIR_SRCS
#AUX_SOURCE_DIRECTORY(. DIR_SRCS)

# 如果不是static库, 就需要用这句话, 将所有.cpp or .c 文件添加到DIR_SRCS中.
# 但是这个文件夹本身用来放.a static 库文件, 需要包含, 需要INCLUDE_DIRECTORIES().
# 但不需要AUX_SOURCE_DIRECTORY() 添加源文件, passed
