#
# 1.安装libzmq with apt-get system(直接在debian 中安装, 更省事)
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
# 2.编译安装libzmq 库
#
libzmq_src="./libzmq-4.3.4"

cd $libzmq_src

./autogen.sh

# --with-api-poller=auto (自动选择即可, 会自动选择的)
# configure: Using 'epoll' I/O thread polling system with CLOEXEC
# configure: Using 'poll' zmq_poll(er)_* API polling system
# --prefix="/usr/local" 安装在其他路径, 找不到-lzmq, 还要解决$PATH 全局路径的问题, 没必要搞那么麻烦了;
./configure --prefix="/usr/local" --with-gcov=no --with-militant --without-docs --without-documentation --with-api-poller=auto

# 重建local 的pkgconfig 路径
ln -s /usr/local/lib/pkgconfig/ /usr/local/share/




