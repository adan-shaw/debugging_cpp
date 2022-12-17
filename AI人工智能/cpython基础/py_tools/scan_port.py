#coding=utf-8



import sys, socket, time, argparse

# 兼容python2/3
if (sys.version_info.major == 2):
	import thread as pth

if (sys.version_info.major == 3):
	import threading as pth

# 实在解决不了threading 线程库的差异, 禁止python3 运行;
if (sys.version_info.major != 2):
	print("this program is not gonna work on python3, please using python2 to run this program")
	quit()



# 设置socket 默认超时时间
socket.setdefaulttimeout(4)



# 根据IP和TCP端口号, 判断TCP端口是否可用
def scan_tcp_port(ip, port):
	try:
		s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		tmp=s.connect_ex((ip, port))
		if tmp==0:
			print(ip,port,'alive!!')
		s.close()
	except:
		print(ip,port,'execption!!')



# 输入IP, 扫描IP的0-65534 TCP端口监听情况
def scan_all_port(ip):
	try:
		print('start scan', ip)
		t_start=time.time()
		for i in range(0,65534):
			#lock.acquire()
			pth.start_new_thread(scan_tcp_port,(ip, int(i)))
			#lock.release()
		print('scan finished, time-consuming：%.2f' %(time.time()-t_start))
	except:
		print('scan execption!!', ip)



if __name__=='__main__':
	# 初始化命令行解析模块
	parser = argparse.ArgumentParser()
	# 添加命令行选项
	parser.add_argument('--url', type=str, default="127.0.0.1")
	# 解析命令行选项
	args = parser.parse_args()
	print(args)

	# 初始化锁
	#lock=pth.allocate_lock()
	# 开始执行任务
	scan_all_port(args.url)

