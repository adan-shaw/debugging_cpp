#coding=utf-8

from selenium import webdriver



# ps: 打开新的标签页, 需要js 代码协助!!



# 获取Chrome 的选项参数
chrome_options = webdriver.ChromeOptions()

# 根据Chrome 的选项参数, 启动web 浏览器
browser = webdriver.Chrome(chrome_options=chrome_options)

browser.implicitly_wait(2)



# 访问url
browser.get('http://127.0.0.1:8072/')

# 利用js 代码, 创建新Tab标签页
browser.execute_script('window.open("http://127.0.0.1:8072/");')


# 获取所有Tab标签页的句柄
handles = browser.window_handles

# 切换到新的Tab标签页(队列尾=新建Tab标签页=-1)
browser.switch_to.window(handles[-1])

browser.get('http://www.localhost:8072/')



# 切换回第一个Tab标签页
browser.switch_to.window(handles[0])





# 暴力创建128 个
for x in range(128):
	browser.execute_script('window.open("http://127.0.0.1:8072/");')

# 快速关闭128 个(从最后一个开始关闭)
for x in range(128):
	handles = browser.window_handles
	browser.switch_to.window(handles[-1])
	# 不能应该直接用下标进行索引, 因为关掉一个Tab标签页, 下标就会有缩减, 不安全(除非你倒序关闭)
	#browser.switch_to.window(handles[x])
	browser.close()

browser.quit()



# 同一个浏览器, 为什么不能来回刷两次? why? [未知bug]
chrome_options = webdriver.ChromeOptions()
browser = webdriver.Chrome(chrome_options=chrome_options)
browser.implicitly_wait(2)

# 暴力创建128 个
for x in range(128):
	browser.execute_script('window.open("http://www.localhost:8072/");')

# 快速关闭128 个(从第一个开始关闭)
for x in range(128):
	handles = browser.window_handles
	browser.switch_to.window(handles[0])
	browser.close()

browser.quit()
