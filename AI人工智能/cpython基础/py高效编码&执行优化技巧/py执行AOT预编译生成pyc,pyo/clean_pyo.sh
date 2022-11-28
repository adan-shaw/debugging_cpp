#!/bin/sh

project_home="./project_test";
#project_home="./odoo-8.0-20171001";



# 查找并删除-匹配到的"*.pyc"文件
find $project_home -name "*.pyc" -exec rm {} \;


# 查找并删除-匹配到的"*.pyo"文件
find $project_home -name "*.pyo" -exec rm {} \;
