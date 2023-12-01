#!/bin/sh

# 自动生成*.pro [以当前文件夹名为name, 生成: <文件夹name>.pro]
qmake -project -t app


# 覆盖*.pro
#cp ./qmake_demo.bak.pro qmake_demo.pro

# 添加编译宏到*.pro
echo -e "\n\nINCLUDEPATH += ./widget" >> ./quick_demo.pro
echo -e "\n\nLIBS += -L ./widget -l Qt5Widgets" >> ./quick_demo.pro


# 自动生成makefile
qmake -makefile ./quick_demo.pro


# 执行makefile
make
