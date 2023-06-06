#coding=utf-8

from selenium import webdriver





#
# 1.测试Chrome浏览器
#
# 启动Chrome浏览器
browser = webdriver.Chrome()

# 设置访问url 的无响应超时为2 秒
browser.implicitly_wait(2)

# 访问url
browser.get('http://127.0.0.1:8072/')
browser.get('http://www.localhost:8072/')

# 后退
browser.back()

# 刷新(无刷新功能)
#browser.refersh()

# 前进
browser.forward()

# 关闭当前Tab页
browser.close()

# 退出整个Chrome浏览器
browser.quit()





#
# 2.测试Chrome浏览器
#
# 启动Firefox浏览器(没有firefox webdriver, 启动不了)
browser2 = webdriver.Firefox()

browser2.implicitly_wait(2)

browser2.get('http://127.0.0.1:8072/')
browser2.get('http://www.localhost:8072/')

browser2.back()

#browser2.refersh()

browser2.forward()

browser2.close()

browser2.quit()





# 启动IE浏览器(没有IE webdriver, 启动不了):
#browser3 = webdriver.Ie()
#browser3.get('http://127.0.0.1:8072/')
