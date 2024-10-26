# -*- coding: UTF-8 -*-

import pandas as pd
from matplotlib import pyplot as plt
import numpy as np



#
# 没execl 数据, 等于白搭!!
#
def deal(filename):
	# 解决中文显示问题
	plt.rcParams['font.sans-serif']=['SimHei']
	plt.rcParams['axes.unicode_minus'] = False
	# 设置画布大小
	fig =plt.figure(figsize=(16, 6), dpi=400)
	# 读取文件
	df=pd.read_excel("data.xlsx")
	# 单独一列, 按列转换为list
	station=df['站点'].tolist()
	df=df.drop('站点',axis=1)
	# 将表格整体转换为list
	y=np.array(df)
	x=[]
	for i in range(4,23):
		x.append(i)
	# 加个空格以便区分前后数字
	for i in range(4,23):
		x.append(str(i)+' ')
	for i in range(len(y)):
		plt.plot(x,y[i],label=station[i])
	# 求二维list最大值
	miny = max(max(row) for row in y)
	miny=miny-0.05*miny
	# 放置图例
	plt.legend(loc='center',ncol=9,bbox_to_anchor=(0.5, -0.2))
	# 放置文字
	plt.text(16, miny,'进站', ha='center')
	plt.text(21, miny,'出站', ha='center')
	# 放置垂直线
	plt.axvline(18.5,linestyle='--', color='gray')
	# 导出图片
	fig.savefig(filename+".png",bbox_inches = 'tight')



L=['一类站点','二类站点','三类站点','四类站点','五类站点']

for i in range(len(L)):
	deal(L[i])
