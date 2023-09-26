#coding=utf8



import sys
import xlwt
from os.path import isfile

# 添加当前路径到'py模块的搜索路径'中(py进程环境变量)
sys.path.append(".")
# 导入检查sheet 是否存在的API
from py_xlrd_r import excel_if_xls_sheet_name
#from py_xlrd_r import excel_if_xls_sheet_index
# 导入获取sheet rows & cols 的API
from py_xlrd_r import excel_get_xls_sheet_rows_cols



# 本文档声明:
# 	[当execl 数据不能放入python-list 时, 会失败, 这意味着不能存放图片, 只能是: string/number/date]



# 写入数据安全检测(必须为一个完整的二维表, 否则报错)
def excel_write_xls_safe_check(table_data):
	if type(table_data) is not list:
		print("excel_write_xls_safe_check() failed: table_data parameter-type error!!", type(table_data))
		return False
	else:
		for plist_tmp in table_data:
			if type(plist_tmp) is not list:
				print("excel_write_xls_safe_check() failed: plist_tmp parameter-type error!!", type(plist_tmp))
				return False
		return True



# 写入xls格式文件by sheet_index [不能根据index 创建sheet, 必须添加sheet_name, 所以这里弃用]



# 写入xls格式文件by sheet_name
'''
参数:
	url:文件路径
	sheet_name:表名(表名不存在则创建, 存在则追加, 追加方式使用list 先读出数据, 组建新的list, 再写入)
	table_data:将要写入表格的数据(二维列表list)
'''
def excel_write_xls_by_sheet_name(url,sheet_name,table_data):
	if(not isfile(url)):
		print("excel_write_xls_by_sheet_name(%s) failed: xls/xlsx file path error!!" %url)
		return
	if(not excel_write_xls_safe_check(table_data)):
		return
	workbook = xlwt.Workbook(encoding='utf8')
	# 判断sheet 名是否已经在
	if(excel_if_xls_sheet_name(url,sheet_name)):
		# 已存在, 则获取row,col
		row,col=excel_get_xls_sheet_rows_cols(url,sheet_name)
		worksheet = workbook.get_sheet(sheet_name)
	else:
		row=0
		col=0
		worksheet = workbook.add_sheet(sheet_name)
	for tmp_list in table_data:
		col = 0
		for tmp in tmp_list:
			worksheet.write(row,col,tmp)
			col = col + 1
		row = row + 1
	#workbook.flush_row_data()
	workbook.save(url)
	print("excel_write_xls_by_sheet_name(%s) okay!!" %url)



