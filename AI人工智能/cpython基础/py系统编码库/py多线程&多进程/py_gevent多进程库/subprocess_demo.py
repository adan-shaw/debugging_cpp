# -*- coding: utf8 -*-



import subprocess



# 使用subprocess模块执行ping命令, stdout 通过PIPE 管道传输出来
proc_ping = subprocess.Popen(['ping', '127.0.0.1'], stdout=subprocess.PIPE)

# subprocess 关键类成员的type[python2]
print(type(proc_ping))											# <class 'subprocess.Popen'>
print(type(proc_ping.stdout))								# <type 'file'>
print(type(proc_ping.stdout.readline()))		# <type 'str'>

# subprocess 关键类成员的type[python3]
print(type(proc_ping))											# <class 'subprocess.Popen'>
print(type(proc_ping.stdout))								# <class '_io.BufferedReader'>
print(type(proc_ping.stdout.readline()))		# <class 'bytes'>



# 打印pid
print("* pid = ", proc_ping.pid)

# 询问'子进程'是否结束(优)
if(not proc_ping.poll()):
	print("* subprocess.Popen() is running!! [poll()]")

# 询问'子进程'是否结束(通过询问'子进程'返回值, 来得知'子进程'是否结束, 不推荐)
if(not proc_ping.returncode):
	print("* subprocess.Popen() is running!! [returncode 返回值]")



quit_count=4
# 打印'子进程'的stdout(stdout 通过PIPE 返回给'父进程')
while(True):
	stdout = proc_ping.stdout.readline()
	if(stdout == b'' and proc_ping.poll()):
		break

	if(stdout):
		line = stdout.strip().decode('utf-8').strip()
		print("* ", line)

	if(quit_count <= 0):
		break
	else:
		quit_count-=1



# 强制停止'子进程'(不会马上杀死, 必须使用wait() 等待一下)[terminate() 未杀死'子进程', 不进行kill() 则会有僵尸'子进程')
proc_ping.terminate()

# 等待'子进程'结束
proc_ping.wait()

# 打印'子进程'结束返回的结果
print("* Popen.returncode():", proc_ping.returncode)

# 强制杀死'子进程'
proc_ping.kill()

# 打印'子进程'结束返回的结果
print("* Popen.returncode():", proc_ping.returncode)



