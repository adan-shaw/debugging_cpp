#!/bin/sh

# 注意:
#   在posix shell 中, 裁定':' 函数名为非法!!
#   所以这个fork_bomb 简式, 实际只能在bash shell 中执行!!
#   :(){:|:&};:
#   想要在posix shell中, 运行fork_bomb.sh, 直接将':'换成合法字母即可.
#   例如: a(){a|a&};a



# 1.fork bomb 原型:
#:(){:|:&};:



# 2. fork bomb 展开式:
# 2.1 函数定义:
#   递归fork 函数自身(函数名为':'), 并且推到后台执行!!
:<<!
:(){
	:|: &
};
!
# 分割符号';', 是用来隔绝'函数定义体'和'函数执行体'两部分的.

# 2.2 函数执行:
#:



# 3. fork bomb 改版(改名):
# 函数体定义:
fork_bomb() {
	fork_bomb | fork_bomb &
};

# 函数体执行:
#fork_bomb

echo "不要执行!! 只要函数定义, posix shell过关, 调用体可以免去!! 免得关机"
exit "0"

