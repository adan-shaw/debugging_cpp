MESSAGE(STATUS "hello, i'm src1 '子CMakeLists.txt'")



#
# 描述二级目录的文件分布详情
#

# 找头文件
INCLUDE_DIRECTORIES(
	./src_src1
	./src_src2
	.
)
