#
# 1.安装libczmq with apt-get system(依赖直接在debian 中安装, 更省事)
#

# 安装libzmq(lightweight messaging kernel)
apt-get install libzmq5 libzmq3-dev

# 安装czmq(High-level C binding)
apt-get install libczmq4 libczmq-dev

# python 版本的libzmq
pypy-zmq - PyPy bindings for 0MQ library

python-zmq - Python bindings for 0MQ library
python-zmq-dbg - Python bindings for 0MQ library - debugging files
python3-zmq - Python3 bindings for 0MQ library
python3-zmq-dbg - Python3 bindings for 0MQ library - debugging files

python-txzmq - ZeroMQ binding for the Twisted framework (Python2 version)
python3-txzmq - ZeroMQ binding for the Twisted framework (Python3 version)





#
# 2.编译安装c++zmq 库(debian 没有c++zmq 库, 需要自己编译安装)
#
cppzmq_srv_src="./cppzmq-4.10.0"

cd $cppzmq_srv_src

# 查看libzmq 的安装路径
ls /usr/lib/x86_64-linux-gnu | grep libzmq
ls /usr/lib/x86_64-linux-gnu/pkgconfig | grep libzmq

# 查看是否存在libzmq(非常重要)
pkg-config --list-all | grep libzmq

mkdir build
cd ./build
cmake .. -L

# 开启-DCPPZMQ_BUILD_TESTS:STRING=NO 需要安装catch2 库(禁用最方便)
cmake -G "Unix Makefiles" .. -DCMAKE_INSTALL_PREFIX:PATH=/usr/local -DCMAKE_BUILD_TYPE:STRING=RELEASE -DCPPZMQ_BUILD_TESTS:STRING=NO

# cppzmq 是一个c++ 编码库, 没有运行程序, 直接安装就只有zmq.hpp 头文件而已, 很废物!!
# 所以编译瞬间秒杀, 根本不需要编译, 也没有.exe 文件
# 编译时, 也只是编译测试模块而已(可以不编译测试模块), 没有什么实际意义, .so 共享链接库都不会有!! cppzmq 就是一个对libzmq 二次封装的东西, 只有*.hpp头文件!!





# 不确定(应该可以找到, 阅读CMakeLists.txt 第13-16 行, 找不到libzmq, 他会自己找, 不用担心)
# debian 安装libzmq 库, 安装到/usr/lib/x86_64-linux-gnu 里面了, 比较乱, -DCMAKE_PREFIX_PATH=/usr 找不到, 只能手动编译libzmq 了(必须手动编译libzmq 库, c++zmq 不是czmq !!)
