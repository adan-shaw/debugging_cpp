#coding=utf-8



# 写入xlsx格式文件
'''
参数:
	url:文件路径
	sheet_name:表名
	table_data:将要写入表格的数据(二维列表list)
'''
def write_xlsx_excel(url, sheet_name, table_data):
	workbook = openpyxl.Workbook()
	sheet = workbook.active
	sheet.title = sheet_name
	for i in range(0, len(table_data)):
		for j in range(0, len(table_data[i])):
			sheet.cell(row=i + 1, column=j + 1, value=str(table_data[i][j]))
	workbook.save(url)
	print("write_xlsx_excel() okay!!")



# 追加写入xlsx格式文件
'''
参数:
	url:文件路径
	sheet_name:表名
	table_data:将要写入表格的数据(二维列表list)
'''
def write_xlsx_excel_add(url, sheet_name, table_data):
	workbook = openpyxl.load_workbook(url)
	sheet = workbook[sheet_name]
	for tmp in table_data:
		sheet.append(tmp)
	workbook.save(url)
	print("write_xlsx_excel_add() okay")
