#coding=utf-8



import sys
from os import times



# 添加当前路径到'py模块的搜索路径'中(py进程环境变量)
sys.path.append(".")

from py_xlrd_r import excel_read_xls_by_sheet_index
from py_xlrd_r import excel_read_xls_by_sheet_name
from py_xlwt_w import excel_write_xls_by_sheet_name



# 读
#plist=excel_read_xls_by_sheet_index("./old.xls", 1)
#print(type(plist), plist)

plist=excel_read_xls_by_sheet_name("./old.xls", "Sheet2")
print(type(plist), plist)



# 写
plist2=[[1,2],[11,12],[13,14],[15,16],[17,18],[19,00]]
tmp=[]
tmp.append(times())
tmp.append(times())
plist2.append([99,99])
#plist2.append(tmp)
excel_write_xls_by_sheet_name("./old.xls", "Sheet3", plist2)

# 追加写
excel_write_xls_by_sheet_name("./old.xls", "Sheet2", plist2)

# 检查写结果
print("\n\n\n")
print("*******************check writing results*******************")
plist3=excel_read_xls_by_sheet_name("./old.xls", "Sheet2")
plist4=excel_read_xls_by_sheet_name("./old.xls", "Sheet3")
print(type(plist3), plist3)
print(type(plist4), plist4)
print("*******************check writing results*******************")
