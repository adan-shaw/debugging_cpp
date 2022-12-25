#coding=utf-8



# python 的enum 枚举结构简介
'''
	python 并没有enum 枚举结构;

	python 的enum 枚举体的实现, 使用类定义来实现的, 
	然后利用类定义, 实现常量名替换, 实现枚举;
	(有点类似宏定义替换字符串, 实际作用于python 解析器, 并不会产生任何实质性的内存;
	 虽然python 是边翻译, 边运行, 但实际原理都差不多, 也是需要编译, 运行的;)



	python 3.4之前, 并没有关于enum 枚举体的相关实现;

	python 3.4中新增加了'enum 枚举类'(但实际也没有多大改进, 'enum 枚举类'同样也是只能在python 解析器中存活), 
	通过继承'enum 枚举类', 得到一个功能更强的派生枚举类, 比手工定义的枚举类, 功能强很多;

	'enum 枚举类'和普通类的用法不同:
		'enum 枚举类'不能用来实例化对象, 但这并不妨碍我们在编程的时候, 访问枚举类中的成员;
		('enum 枚举类'在python 程序运行过程中, 是不存在的; 类实体在python 程序运行过程中, 是存在类实体的)



	'enum 枚举类'中应该遵守的规则:
		1、枚举类中, 不应该存在key相同的枚举项(类变量)
		2、不允许在类外直接修改枚举项的值(因为enum枚举类是没有实体的, 因此没办法修改)
'''



# 定义'enum 枚举类'
class Directions:
	NORTH = 1
	EAST = 2
	SOUTH = 3
	WEST = 4



# 引用'enum 枚举类'的成员
print(Directions.NORTH,Directions.EAST,Directions.SOUTH,Directions.WEST)



# 查看'enum 枚举类'中是否有某个成员(不存在会直接报错!! 存在会返回False, 因为毕竟没有类实体, 只有类定义)
print(isinstance(Directions.EAST, Directions))


