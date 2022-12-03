#coding=utf-8



# cpython 2.7以上的版本
import sysconfig;
print(sysconfig.get_config_vars());



# cpython 2.7以下的版本
import distutils.sysconfig;
print(distutils.sysconfig.get_config_vars());



# pypy
#pypy --info
