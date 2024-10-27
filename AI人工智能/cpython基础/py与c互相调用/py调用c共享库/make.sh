#!/bin/sh

# 执行编译
gcc -fPIC -shared ./sum.c -o ./sum.so

# 执行测试
python sum_test.py
