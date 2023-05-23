#coding=utf-8



import hashlib



# 0.md5 与sha1 校验算法的比较
'''
	由于MD5与SHA-1均是从MD4发展而来, 它们的结构和强度等特性有很多相似之处;
	SHA-1与MD5的最大区别在于其摘要比MD5摘要长32 bit;
	对于强行攻击, 产生任何一个报文使之摘要等于给定报文摘要的难度:
		MD5是2128数量级的操作;
		SHA-1是2160数量级的操作;
	产生具有相同摘要的两个报文的难度:
		MD5是264是数量级的操作;
		SHA-1是280数量级的操作;
	因而, SHA-1对强行攻击的强度更大;
	但由于SHA-1的循环步骤比MD5多80:64且要处理的缓存大160比特:128比特, SHA-1的运行速度比MD5慢;
'''





# 1.单纯计算md5 for 校验(不做隐藏, 可根据md5 逆向计算出src 字符串)
'''
	仅支持字符串, 方便json/xml/string 做md5校验, 
	file/二进制文件校验, 请直接用md5sum 命令;
	md5sum 校验命令: 
		echo -n "i am adan shaw" | md5sum
	ps:
		echo 默认会把'\n'换行符输入到md5sum, 
		所以必须加入-n 正则表达式选项, 去除'\n'换行符号, 
		否则md5 校验码永远算不对;
'''
def md5_check(val: str) -> str:
	md5 = hashlib.md5()
	md5.update(val.encode('utf-8'))
	return md5.hexdigest()

pstr="i am adan shaw"
print("md5_check(): ",md5_check(pstr))





# 2.复杂计算md5 for 校验(双端动态约定, 自动加入打乱因子, 让md5 无法逆向计算出src 字符串)
'''
最优策略:
	加入动态+静态打乱因子, 最完美;

动态打乱因子:
	为了防止空字符串崩溃, 动态打乱因子, 采用字符串len()->str 作为因子;
	len(str(None)) 不会崩溃, 而是python 自动强转None 为字符串, 稳;

静态打乱因子:
	adan_shaw
'''
def md5_checkEx(val: str) -> str:
	md5 = hashlib.md5(str(len(val)).encode('utf-8')+"adan_shaw".encode('utf-8'))
	md5.update(val.encode('utf-8'))
	return md5.hexdigest()

pstr="i am adan shaw"
print("md5_checkEx(): ",md5_checkEx(pstr))





# 3.md5 用途
'''
md5 不能用作加密?
md5 的确不能用作加密, 因为md5 不具备复原性!!

对称加密的意义在于:
	可加密
	可复原

而md5 是不能复原的, 因此不能把md5 当成对称加密算法来用;

但, md5 的实际用途是:
	客户端输入了密码, 在'密码/密文'不传输到internet 的情况下, 与server 完成密码校验, 查看密码是否正确输入;
	实现流程:
		client:
			根据'固定打乱式md5', 计算出md5 校验码, 发送到server;
		server:
			根据'固定打乱式md5', 结合数据库里面存放的passwd, 计算出md5 校验码, 
			与client 发上来的数据进行对比, 100% 吻合, 则判断用户输入的密码正确;

缺点:
	如果不使用https, 人家同样可以直接截获你的md5 码, 改都不改, 直接挪用;
	而你又不能根据time 来动态生成md5, 这样做意义不大, 还要做时间对齐;
	(这样系统会非常不可靠, 整天要对齐时间, 要做时间同步)
	最可靠的方式, 还是https + md5 适当加密;

缺点修正方法(failed):
	因此, 必须有两个md5:
		1.计算密码, 存放到md5
		2.计算整个报文, 保证数据完整性? (没用, 人家照样在封包的时候, 重新对整个数据包, 再算一个md5, 别浪费时间和性能)
'''




