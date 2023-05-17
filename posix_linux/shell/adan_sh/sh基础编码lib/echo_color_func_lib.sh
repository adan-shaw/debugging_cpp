#!/bin/sh


# 本文件是shell echo 回显彩字的函数库, 已经封装好的.
# 使用彩字回显, 必须使用echo -e !!



##########################################################

#
# 声明: 一律只打印一串字符串, $1!! 多出的统一不操作.
#

# 黑底红字
fonts_red() { echo -e "\033[31m $1 \033[0m"; return; }
# 黑底绿字
fonts_green() { echo -e "\033[32m $1 \033[0m"; return; }
# 黑底黄字(橙色)
fonts_yellow() { echo -e "\033[33m $1 \033[0m"; return; }
# 黑底蓝字
fonts_blue() { echo -e "\033[34m $1 \033[0m"; return; }
# 黑底紫字
fonts_purple() { echo -e "\033[35m $1 \033[0m"; return; }
# 黑底青字
fonts_cyan() { echo -e "\033[36m $1 \033[0m"; return; }
# 黑底白字(原封不动)
fonts_white() { echo -e "$1"; return; }


# 红底白字
background_red() { echo -e "\033[41;37m $1 \033[0m"; return; }
# 黄底白字
background_yellow() { echo -e "\033[43;37m $1 \033[0m"; return; }
# 蓝底白字
background_blue() { echo -e "\033[44;37m $1 \033[0m"; return; }
# 紫底白字
background_cyan() { echo -e "\033[45;37m $1 \033[0m"; return; }
# 白底黑字
background_white() { echo -e "\033[47;30m $1 \033[0m"; return; }


##########################################################






# test 测试函数
color_test() {
	# 不换底色, 换字色
	echo -e "\033[30m 黑色字 \033[0m"
	echo -e "\033[31m 红色字 \033[0m"
	echo -e "\033[32m 绿色字 \033[0m"
	echo -e "\033[33m 黄色字 \033[0m"
	echo -e "\033[34m 蓝色字 \033[0m"
	echo -e "\033[35m 紫色字 \033[0m"
	echo -e "\033[36m 天蓝字 \033[0m"
	echo -e "\033[37m 白色字 \033[0m"

	# 换底色, 不换字色(37m 字色 = 白色)
	echo -e "\033[40;37m 黑底白字 \033[0m"
	echo -e "\033[41;37m 红底白字 \033[0m"
	echo -e "\033[42;37m 绿底白字 \033[0m"
	echo -e "\033[43;37m 黄底白字 \033[0m"
	echo -e "\033[44;37m 蓝底白字 \033[0m"
	echo -e "\033[45;37m 紫底白字 \033[0m"
	echo -e "\033[46;37m 天蓝底白字 \033[0m"
	echo -e "\033[47;30m 白底黑字 \033[0m"

	# my function test
	fonts_red "love you "
	fonts_green "love you "
	fonts_yellow "love you "
	fonts_blue "love you "
	fonts_purple "love you "
	fonts_cyan "love you "
	fonts_white "love you "

	background_red "love you "
	background_yellow "love you "
	background_blue "love you "
	background_cyan "love you "
	background_white "love you "

	return
}



# 执行测试
color_test;


