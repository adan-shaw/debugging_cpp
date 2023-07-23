MESSAGE(STATUS "hello, i'm dynamic_lib '子CMakeLists.txt'")



#
# 描述二级目录的文件分布详情
#

# 找头文件
INCLUDE_DIRECTORIES(
	./headfile
	.
)

