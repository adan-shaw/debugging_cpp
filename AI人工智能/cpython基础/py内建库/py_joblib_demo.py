#coding=utf-8



import joblib



# 安装
# sudo apt-get install python-joblib
# sudo apt-get install python3-joblib



pdict={"name": "小李", "sex": "男", "tel": ["8234588", "1244351"], "isonly": True}

# py 数据导出保存
path_saved = joblib.dump(pdict,"./pdict.pkl")

# 从./pdict.pkl 文件中导入数据
pdict2 = joblib.load("".join(path_saved))

print(pdict2)
