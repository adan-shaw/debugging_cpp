mkdir build

cd build

# 必须依赖系统, 找到编译器, 否则报错!!
cmake -G "NMake Makefiles" ..

# 强制跳过c/c++ 编译器检查, windows 平台不允许这么做, 否则<iostream> 都找不到, 非常恶心;
#cmake -D CMAKE_CXX_COMPILER_WORKS=TRUE -D CMAKE_C_COMPILER_WORKS=TRUE -G "NMake Makefiles" ..
#cmake -D CMAKE_CXX_COMPILER_WORKS=TRUE -G "NMake Makefiles" ..
#cmake -D CMAKE_CXX_ABI_COMPILED=TRUE -D CMAKE_C_ABI_COMPILED=TRUE -G "NMake Makefiles" ..
#cmake -D CMAKE_CXX_ABI_COMPILED=TRUE -G "NMake Makefiles" ..

# 如果c/c++ 编译器找不到, 请检查vc++ 的安装是否有问题

nmake -A

cd ..
