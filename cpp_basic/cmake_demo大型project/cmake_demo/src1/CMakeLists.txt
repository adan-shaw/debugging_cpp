MESSAGE(STATUS "hello, i'm src1 '子CMakeLists.txt'")



#
# 描述二级目录的文件分布详情
#

# 本层CMakeLists.txt 的结构描述体(描述本层目录的文件&文件夹详情)
INCLUDE_DIRECTORIES(
	./src_src1
	./src_src2
	.
)
