//运行:
//		java ./main.java



//1.java 机的特点(简介):
/*
	*Java 里面的注释, 跟C++ 是一样;

	*Java 的所有进程, 线程, 都在jvm 中实现, 只提供jdk 供程序员调用;
		所以, 任何java 程序都离不开jvm, 既是优点, 也是缺点;
		所以, 维护好jvm, 可以提高所有java 程序的执行效率, 但如果你不是骨灰级java 程序员, 使用默认jvm 即最好;

	*Java 所有的功能包, 都内部嵌入到jvm 中, 以实现jdk API;
		因此, 你不需要担心java 程序引用了多少个java 包, 不会对java 程序有任何实质性的影响;

	*Java 程序执行前, 都会进行预编译, 以提高java 程序的运行性能;
		(当然java 不能100% 全部编译所有代码, 只能编译大部分代码, 尽量提高java 程序的执行效率)

	*Java 的编码风格, 跟c++ 非常相似, 面向对象编程老玩家了;

	*Java 支持c++ 的大部分功能, 编码细节也跟c++ 一样, 多线程, 多进程, 非常相似;
		一定程度上,Java 约等于c++ 编码;

	* 脚本编程性能要义: 最小化包含战术,Java 同样适用;
		大量使用System.out.* 系统自带包, 不如自定义一个import, 这样无论是解析, 还是编译, 都可以做到性能最优;
		当然java 可以先编译, 后运行, 
		这种优化, 可能只能节约编译时间, 不能节约运行时间, 
		这可能是java 比python/lua 强的地方, 不需要太在意写法的性能优化问题;
		(但python 也有编译功能, 一定程度上, 编译不能完全解决这种问题, 
		 所以, 还是坚守脚本语言的优化战术为好: 最小化包含战术)
*/



//2.面向对象编程要义:
/*
	每一个.java 文件, 有且只有一个类, 方便做堆叠;
	哪怕是子类继承, 也单独出来一个文件, 这样索引关系比较清晰;
	(如果子类太多, 但改动不大, 合并到一个文件里面? 这种情况真的比较少, 别幻想)
	坚持: 一个.java 文件, 一个子类;
*/



//3.java 常用的系统包
/*
	包											说明
	java.lang							Java 的核心类库, 包含运行Java 程序必不可少的系统类, 如基本数据类型、基本数学函数、字符串处理、异常处理和线程类等, 
												系统默认加载这个包;
	java.io								Java 语言的标准输入/输出类库, 如基本输入/输出流、文件输入/输出、过滤输入/输出流等;
	java.util							包含如处理时间的Date 类, 处理动态数组的Vector 类, 以及 Stack 和 HashTable 类
	java.awt							构建图形用户界面(GUI)的类库, 低级绘图操作 Graphics 类、图形界面组件和布局管理
												(如: Checkbox 类、Container 类、LayoutManger 接口等), 
												以及用户界面交互控制和事件响应(如: Event 类);
	java.awt.image				处理和操纵来自网上的图片的Java 工具类库
	java.wat.peer					很少在程序中直接用到, 使得同一个Java 程序在不同的软硬件平台上运行
	java.net							实现网络功能的类库有 Socket 类、ServerSocket 类
	java.lang.reflect			提供用于反射对象的工具
	java.util.zip					实现文件压缩功能
	java.awt.datatransfer	处理数据传输的工具类, 包括剪贴板、字符串发送器等
	java.sql							实现JDBC 的类库
	java.rmi							提供远程连接与载入的支持
	java. security				提供安全性方面的有关支持

	java 的编码风格, 是庞大的System.out, System.err, ... 这种包前缀名没办法去掉的, 
	所以java 的编码离不开IDE 工具的, 有IDE 工具很方便, 没有IDE 工具就是地狱, 自己看着办;
*/



//4.java 默认的入口函数main()
/*
	由于java 程序全部是装载进jvm 中运行的, 
	因此, jvm 绝对不允许有任何子java 程序, 导致jvm 崩溃, 
	因此, java 的默认入口程序, 一般被设计为:
		public static void main(String args[]){}

	无返回, 没办法获取执行状况, 子java 程序是否执行成功, 都不会对jvm 有任何影响;
*/



//5.局部变量是否需要前置
/*
	java 是有局部变量自动回收机制的, 有内存回收gc 系统, 
	因此, 不需要在意局部变量是否前置, 随便玩即可;
*/



//包含java.lang 中的System 包
import java.lang.System;

public class main{
	//java 的入口函数
	public static void main(String args[]){

		System.out.println("遍历main() 中的args输入参数:");
		for(int i=0;i<args.length;i++)
			System.out.println(args[i]);

		//获取Java程序相关联的运行时对象
		Runtime rt = Runtime.getRuntime();
		System.out.println("处理器数量:" + rt.availableProcessors());
		System.out.println("空闲内存数:" + rt.freeMemory());
		System.out.println("总内存数:" + rt.totalMemory());
		System.out.println("可用最大内存数:" + rt.maxMemory());

		//异常捕抓
		try{
			int a[] = {1,2,3};
			a[11] = 1;
		}
		catch(Throwable e){
			System.out.println(e.toString());
		}

		return;
	}
}


