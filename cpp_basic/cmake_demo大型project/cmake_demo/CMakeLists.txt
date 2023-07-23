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




MESSAGE(FATAL_ERROR "quit")


