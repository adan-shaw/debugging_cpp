#coding=utf8

import sys

if(sys.version_info.major == 3):
	# 少于3.3 跟2.7 类似
	if(sys.version_info.minor <= 3):
		from urllib import quote
	else:
		from urllib.parse import quote

if(sys.version_info.major == 2):
	from urllib import quote

#from urllib.parse import quote
#from urllib import quote



keyword='自学'
url='https://www.baidu.com/s?wd='+quote(keyword)
print(url)
