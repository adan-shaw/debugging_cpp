#coding=utf8



# 读取xlsx格式文件
'''
参数:
	url:文件路径
	sheet_name:表名
返回[list]:
	data:表格中的数据
'''
def read_xlsx_excel(url, sheet_name):
	workbook = openpyxl.load_workbook(url)
	sheet = workbook[sheet_name]
	data = []
	for row in sheet.rows:
		da = []
		for cell in row:
			da.append(cell.value)
		data.append(da)
	return data
