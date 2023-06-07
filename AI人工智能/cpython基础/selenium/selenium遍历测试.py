#coding=utf-8



from selenium import webdriver



# 测试主要的url home
url_home="http://www.localhost:8072/"



pset4js=set()
pset4url=set()



# [递归/不递归]测试一页中的所有links, 并将这些links 添加到全局set: pset4js, pset4url中
def test_all_links(browser, url, pset4js, pset4url):
	# 打开首页
	browser.get(url)
	# 获取首页下所有的<a> 标签
	links = browser.find_elements_by_tag_name("a")
	#print(type(links))
	# 统计<a> 标签list 的个数
	links_count = len(links)

	for i in range(0,links_count):
		#print(type(links[i]))
		if links[i].is_displayed():
			# 获取<a> 标签指向的url
			tmp=links[i].get_attribute("href")

			if(tmp not in pset4url):
				# 添加该url 字符串到pset4url 里面
				pset4url.add(tmp)

				# 组装js + url 字符串, 方便web 浏览器打开该url
				url_tmp="\'window.open(%s);\'" %(tmp)

				# 添加该js + url 字符串到set 里面
				if(url_tmp not in pset4js):
					pset4js.add(url_tmp)

				# 递归遍历这个新的url[不递归, 就是仅打印一页中所有的<a>标签]
				#test_all_links(browser, tmp, pset4js, pset4url)
				print(url_tmp)



chrome_options = webdriver.ChromeOptions()
browser = webdriver.Chrome(chrome_options=chrome_options)
browser.implicitly_wait(0)
# 弃用firefox
#browser = webdriver.Firefox()



# 测试一页中的所有links
test_all_links(browser,url_home,pset4js, pset4url)

print(pset4js)
print(pset4url)

# 关闭浏览器
browser.quit()
