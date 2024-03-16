mkdir build

cd build

# 转换为nmake, 可以直接nmake(必须依赖系统, 找到编译器, 否则报错)
cmake -G "NMake Makefiles" ..

# 转换为vs IDE project, 不能直接编译
#cmake -G "Visual Studio 14 2015 Win64" ..

# 强制跳过c/c++ 编译器检查, windows 平台不允许这么做, 否则<iostream> 都找不到, 非常恶心;
#cmake -D CMAKE_CXX_COMPILER_WORKS=TRUE -D CMAKE_C_COMPILER_WORKS=TRUE -G "NMake Makefiles" ..
#cmake -D CMAKE_CXX_COMPILER_WORKS=TRUE -G "NMake Makefiles" ..
#cmake -D CMAKE_CXX_ABI_COMPILED=TRUE -D CMAKE_C_ABI_COMPILED=TRUE -G "NMake Makefiles" ..
#cmake -D CMAKE_CXX_ABI_COMPILED=TRUE -G "NMake Makefiles" ..

# 如果c/c++ 编译器找不到, 请检查vc++ 的安装是否有问题

nmake -A

cd ..



# 不用cd, 可以用-S <CMakeLists.txt dir> -B <build dir> 来指定路径
#cmake -S . -B vs_project -G "Visual Studio 14 2015 Win64"

#cmake -S . -B build -G "NMake Makefiles"

# 强制指定64 bit
#cmake -S . -B build -A x64 -G "NMake Makefiles"
#cmake -S . -B build -DCMAKE_GENERATOR_PLATFORM=x64 -G "NMake Makefiles"



# w7 不知道怎么指定32bit platform, 使用32bit 编译exe, 链接boost 会报错: boost 库版本是64bit 版本;
# nmake makefiles 不知道怎么指定64bit 版本;
# 32/64 bit 版本不对, 链接失败很正常;
# in windows platform, 32/64 兼容版本即32bit 版本, 滑稽;

# 实在不行, 转换成"Visual Studio 14 2015 Win64", 再编译也是可以的;
