#!/bin/sh


# bug: 不知道为什么, 一执行*.SerializeToOstream(), *.SerializeToString(), 就会报错!! 而且永远是: 
:<<!
terminate called after throwing an instance of 'std::system_error'
  what():  Unknown error -1
Aborted
!

# 这个问题, 不是main() 函数的问题, 也不是test.proto 文件的问题, 
# 初始化资源出错, 难道是protobuff 版本差异导致的init 初始化失败?
# 崩溃是在main() 执行前就崩溃了, 非常奇怪!!
# 这么说, protobuff 一旦启用, 就会在main() 运行之前初始化部分内容, 如果protobuff 初始化失败就会导致莫名其妙的崩溃!!



# 执行调试
./x
