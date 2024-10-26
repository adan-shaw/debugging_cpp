#coding=UTF-8

import json
import os
import openpyxl
import pandas as pd


def export_excel(export, xlsx_name, order):
	'''
	export: 传入的键值对
	xlsx_name: 表明
	xlsx_name: 名称栏
	'''
	# 将字典列表转换为DataFrame
	pf = pd.DataFrame(list(export))
	# 指定字段顺序
	pf = pf[order]
	# 按照指定(target_key)值排序
	# pf = pf.sort_values('target_key', ascending=True)
	# pf.rename(columns=columns_map, inplace=True)
	# 指定生成的Excel表格名称
	file_path = pd.ExcelWriter(f'{xlsx_name}.xlsx')
	# 替换空单元格
	pf.fillna(' ', inplace=True)
	# 输出
	pf.to_excel(file_path, encoding='UTF-8', index=False)
	# 保存表格
	file_path.save()


def insert_xlxs(filer_name, data, order):
	export_excel(export=data, xlsx_name=os.path.join(filer_name, input_json.replace('.json', '')), order=order)



if __name__ == '__main__':
	project_path = os.path.abspath(os.path.dirname(__file__))
	# input_json 输入文件名(json 字符串只接受压缩字符串, 请到web-json 网站, 整理好, 确保json 字符串没问题, 且压缩正确)
	input_json = 'json2execl2.json'
	win_info = os.path.join(project_path, input_json)

	with open(win_info, 'r', encoding='UTF-8') as fp:
		awarded_data = fp.readlines()

	# 获取列名
	order = [k for k, v in json.loads(awarded_data[0]).items()]
	new_data = [json.loads(i) for i in awarded_data]
	insert_xlxs(filer_name=project_path, data=new_data, order=order)

