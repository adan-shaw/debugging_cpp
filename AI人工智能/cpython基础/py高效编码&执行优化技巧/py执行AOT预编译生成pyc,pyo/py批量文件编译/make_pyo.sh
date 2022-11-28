#!/bin/sh

project_home="./project_test";
#project_home="./odoo-8.0-20171001";



# 全部编译为pyc[装载编译模块, 然后对文件夹执行'遍历式自动编译']
#python -c "import compileall; compileall.compile_dir('$project_home');"


# 全部编译为pyo, 并执行-OO 优化(两个大写字母'O')
py_bin="/usr/bin/python"
find $project_home -name "*.py" -exec $py_bin -OO -m py_compile {} \;
