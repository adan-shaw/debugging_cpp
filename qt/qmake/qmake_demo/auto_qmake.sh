#!/bin/sh

# 自动生成*.pro
qmake -project -t app


# 覆盖*.pro
#cp ./qmake_demo.bak.pro qmake_demo.pro

# 添加编译宏到*.pro
echo -e "\n\nINCLUDEPATH += ./widget" >> ./qmake_demo.pro
echo -e "\n\nLIBS += -L ./widget -l Qt5Widgets" >> ./qmake_demo.pro


# 自动生成makefile
qmake -makefile


# 执行makefile
make
