#coding=UTF-8



import xlrd
from os.path import isfile



# 本文档声明:
# 	[当execl 数据不能放入python-list 时, 会失败, 这意味着不能存放图片, 只能是: string/number/date]



# 读取xls文件by sheet_index
'''
参数:
	url:文件路径
	sheet_index:工作表序号(sheet_index 是从0开始的, 跟Sheet 自动命名从1 开始有点区别; sheet_index 不正确会报错, 十分不可靠)
返回[list]:
	data:表格中的数据
'''
def excel_read_xls_by_sheet_index(url,sheet_index):
	if(not isfile(url)):
		print("excel_read_xls_by_sheet_index(%d) failed: xls/xlsx file path error!!" %sheet_index)
		return None
	book = xlrd.open_workbook(url)
	sheet = book.sheet_by_index(sheet_index)
	data = []
	for i in range(0, sheet.nrows):
		tmp = []
		for j in range(0, sheet.ncols):
			tmp.append(sheet.cell_value(i, j))
		data.append(tmp)
	return data



# 读取xls文件by sheet_name
'''
参数:
	url:文件路径
	sheet_name:工作表名(表名不正确会提示)
返回[list]:
	data:表格中的数据
'''
def excel_read_xls_by_sheet_name(url,sheet_name):
	if(not isfile(url)):
		print("excel_read_xls_by_sheet_name(%s) failed: xls/xlsx file path error!!" %url)
		return None
	book = xlrd.open_workbook(url)
	sheet_names = book.sheet_names()
	if not sheet_name in sheet_names:
		print("excel_read_xls_by_sheet_name(%s) failed: if not sheet_name in sheet_names!!" %sheet_name)
		return None
	sheet = book.sheet_by_name(sheet_name)
	data = []
	for i in range(0, sheet.nrows):
		tmp = []
		for j in range(0, sheet.ncols):
			tmp.append(sheet.cell_value(i, j))
		data.append(tmp)
	return data



# 读取xls文件by sheet_index, 判断sheet_index 是否已存在
'''
参数:
	url:文件路径
	sheet_index:工作表序号(sheet_index 是从0开始的, 跟Sheet 自动命名从1 开始有点区别; sheet_index 不正确会报错, 十分不可靠)
返回:
	无返回, 不存在则报错
'''
def excel_if_xls_sheet_index(url,sheet_index):
	book = xlrd.open_workbook(url)
	sheet = book.sheet_by_index(sheet_index)
	return



# 读取xls文件by sheet_name, 判断sheet_name 是否已存在
'''
参数:
	url:文件路径
	sheet_name:工作表名(表名不正确会提示)
返回:
	sheet_name存在则返回True, 不存在则返回False;
'''
def excel_if_xls_sheet_name(url,sheet_name):
	book = xlrd.open_workbook(url)
	sheet_names = book.sheet_names()
	if not sheet_name in sheet_names:
		return False
	else:
		return True



# 读取xls文件by sheet_name, 获取rows,cols
'''
参数:
	url:文件路径
	sheet_name:工作表名(表名不正确/不存在, 则返回0,0)
返回:
	rows,cols
'''
def excel_get_xls_sheet_rows_cols(url,sheet_name):
	book = xlrd.open_workbook(url)
	sheet_names = book.sheet_names()
	if not sheet_name in sheet_names:
		return 0,0
	else:
		sheet = book.sheet_by_name(sheet_name)
		return sheet.nrows, sheet.ncols





