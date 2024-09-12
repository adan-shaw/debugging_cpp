#!/bin/sh


# bug: 不知道为什么, 一执行*.SerializeToOstream(), *.SerializeToString(), 就会报错!! 而且永远是: 
:<<!
terminate called after throwing an instance of 'std::system_error'
  what():  Unknown error -1
Aborted
!

# 这个问题, 暂时解决不了, 详细阅读一下protobuff 的handbook 细节才能解决



# 执行调试
./x
