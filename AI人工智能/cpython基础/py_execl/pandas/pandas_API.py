#coding=UTF-8



import pandas
import numpy



# pandas 默认规则:
'''
*1:
	pandas 获取的execl 表, 是带索引列的;
	通常情况下, 头部列名, 就是获取列时的关键字, 而头部行名, 通常是自增数字0,1,2,3...等;
	因此, 会出现一个常见的怪异现象: 头部列名是字符串, 头部行名是数字;
	但实际上, 头部行名也可以是字符串;
	总结:
		谁在头部, 谁就是头部列名, 头部行名, 任意类型都可, 排序时会用到的;
		col_ahead  = 头部列名
		col_ahead1 = 头部列名1
		col_ahead2 = 头部列名2
		col_ahead3 = 头部列名3

*2:
	元素阶级说明:
		dataframe = DataFrame对象 = 二维表
		series    = Series对象    = 一维表
		pval      = 单元格中的值(类型不限)
		pint      = 统计值返回
		pstr      = 字符串信息返回
		parr      = 一位数组

*3:
	必须用一个新的dataframe 装载排序后的DataFrame对象, 保存排序的结果;
	否则排序结果会丢失, 不会被保存;
'''




# 0.创建pandas 对象
dataframe=pandas.DataFrame(numpy.random.rand(20,5))		# 根据numpy对象, 创建一个DataFrame对象[numpy初始化为: 20行5列的随机数]
series=pandas.Series(my_list)													# 根据可迭代对象my_list, 创建一个Series对象



# 1.导入数据to pandas, 得到一个DataFrame 对象
dataframe=pandas.read_csv(filename)										# 从CSV文件导入数据
dataframe=pandas.read_table(filename)									# 从限定分隔符的文本文件导入数据
dataframe=pandas.read_json(json_string)								# 从json格式的字符串导入数据
dataframe=pandas.read_html(url)												# 解析URL、字符串或者HTML文件, 抽取其中的tables表格
dataframe=pandas.read_clipboard()											# 从你的粘贴板获取内容, 并传给read_table()
dataframe=pandas.DataFrame(dict)											# 从字典对象导入数据, Key是列名, Value是数据
dataframe=pandas.read_sql(query, conn_object)					# 从SQL表/库导入数据
dataframe=pandas.read_excel(filename)									# 从Excel文件导入数据

# 列操作:
# 	使用usecols参数: 读取第5个工作表的第2列到第5列,可以用下面的代码;
dataframe=pandas.read_excel(r'./test_data.xlsx', sheet_name='sheet工作表name', usecols=[i for i in range (1,6)])

# 列操作:
# 	使用usecols参数: "B,D:E"=读取第1和3到5列, 同时设置参数index_col=1, 把第二列当作索引;
dataframe=pandas.read_excel(r'./test_data.xlsx', sheet_name='sheet工作表name', usecols="A,C:E",index_col=1)

# 行操作:
# 	参数nrows=5: 获取前5行;
# 	参数skiprows=[i for i in range(2,5)]: 同时跳过第2行到第4行; skiprows=[2,3,4]同理;
dataframe=pandas.read_excel(r'./test_data.xlsx', sheet_name='sheet工作表name', skiprows=[2,3,4], nrows=5)



# 2.Pandas DataFrame对象'导出数据'to *
dataframe.to_csv(filename)														# 导出数据到CSV文件
dataframe.to_excel(filename)													# 导出数据到Excel文件
dataframe.to_sql(table_name, conn_object)							# 导出数据到SQL表
dataframe.to_json(filename)														# 以json格式导出数据到文本文件



# 3.查看/检查数据
dataframe=dataframe.head(n)														# 查看DataFrame对象的前n行
dataframe=dataframe.tail(n)														# 查看DataFrame对象的最后n行
pint=dataframe.shape[0]																# 查看行数(固定用法)
pint=dataframe.shape[1]																# 查看列数(固定用法)
pstr=dataframe.info()																	# 查看dataframe 消耗的索引、数据类型和内存信息
pstr=dataframe.describe()															# 查看数值型列的汇总统计
pstr=series.value_counts(dropna=False)								# 查看Series对象的唯一值和计数
pstr=dataframe.apply(pandas.Series.value_counts)			# 查看DataFrame对象中每一列的唯一值和计数[pandas.Series.value_counts 是固定用法, 不能改变]



# 4.数据选取
dataframe2=dataframe[[col_ahead1, col_ahead2]]				# 以DataFrame形式返回多列
series=dataframe[col_ahead]														# 根据列名, 并以Series的形式返回一列
series=dataframe.iloc[0,:]														# 返回第一行
pval=dataframe.iloc[0,0]															# 返回第一列的第一个元素
pval=series.iloc[0]																		# 按位置选取数据
pval=series.loc['index_one']													# 按索引选取数据



# 5.数据清理
dataframe=pandas.isnull()															# 检查DataFrame对象中的空值, 并返回一个Boolean DataFrame对象
dataframe=pandas.notnull()														# 检查DataFrame对象中的非空值, 并返回一个Boolean DataFrame对象
dataframe.columns = ['a','b','c']											# 重命名列名
dataframe.dropna()																		# 删除所有包含空值的行
dataframe.dropna(axis=1)															# 删除所有包含空值的列
dataframe.dropna(axis=1,thresh=n)											# 删除所有小于n个非空值的行
dataframe.fillna(x)																		# 用x替换DataFrame对象中所有的空值
dataframe.rename(columns=lambda x:x+1)								# 批量更改列名
dataframe.rename(index=lambda x:x+1)									# 批量重命名索引
dataframe.rename(columns={'old_name':'new_name'})			# 选择性更改列名
dataframe.set_index('column_one')											# 更改索引列
dataframe.index=pandas.date_range('1900/1/30', periods=dataframe.shape[0])# 给DataFrame对象增加一个日期索引
series.astype(float)																	# 将Series中的数据类型更改为float类型
series.replace(1,'one')																# 用'one'代替所有等于1的值
series.replace([1,3],['one','three'])									# 用'one'代替1, 用'three'代替3



# 6.数据处理Filter, Sort和GroupBy
dataframe[dataframe[col_ahead] > 0.5]																									# 选择col_ahead列的值大于0.5的行
dataframe=dataframe.sort_values(col_ahead1, ascending=False)													# 按照列col_ahead1降序排列数据[必须用一个新的dataframe 装载排序后的DataFrame对象, 保存排序的结果]
dataframe=dataframe.sort_values([col_ahead1,col_ahead2], ascending=[True,False])			# 先按列col_ahead1升序排列, 后按col_ahead2降序排列数据
dataframe.groupby(col_ahead)																													# 返回一个按列col_ahead进行分组的Groupby对象
dataframe.groupby([col_ahead1,col_ahead2])																						# 返回一个按多列进行分组的Groupby对象
dataframe.groupby(col_ahead1)[col_ahead2]																							# 返回按列col_ahead1进行分组后, 列col_ahead2的均值
dataframe.groupby(col_ahead1).agg(numpy.mean)																					# 返回按列col_ahead1分组的所有列的均值
dataframe.apply(numpy.mean)																														# 对DataFrame中的每一列应用函数numpy.mean
dataframe.apply(numpy.max,axis=1)																											# 对DataFrame中的每一行应用函数numpy.max
dataframe.pivot_table(index=col_ahead1, values=[col_ahead2,col_ahead3], aggfunc=max)	# 创建一个按列col_ahead1进行分组, 并计算col_ahead2和col_ahead3的最大值的数据透视表



# 7.数据合并
dataframe.append(dataframe2)													# 将dataframe2中的行添加到dataframe的尾部
dataframe.concat([dataframe, dataframe2],axis=1)			# 将dataframe2中的列添加到dataframe的尾部
dataframe.join(dataframe2,on=col_ahead1,how='inner')	# 对dataframe的列和dataframe2的列执行SQL形式的join



# 8.数据统计
dataframe.describe()																	# 查看数据值列的汇总统计
dataframe.mean()																			# 返回所有列的均值
dataframe.corr()																			# 返回列与列之间的相关系数
dataframe.count()																			# 返回每一列中的非空值的个数
dataframe.max()																				# 返回每一列的最大值
dataframe.min()																				# 返回每一列的最小值
dataframe.median()																		# 返回每一列的中位数
dataframe.std()																				# 返回每一列的标准差



