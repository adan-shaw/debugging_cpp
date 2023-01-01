# 查询多个文件, 不做grep 筛选
find . -name "*.sh" -exec cat -n {} \;



# 查询多个文件, 做grep 筛选, 看看有没有关键字"find"
# (遗憾的不是显示文件名, 只能显示这些文件, 有没有关键字, 但可以显示行号)
find . -name "*.sh" | xargs cat -n | grep "find"
