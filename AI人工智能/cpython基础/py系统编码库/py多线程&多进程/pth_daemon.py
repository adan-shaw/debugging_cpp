#coding=UTF-8



import threading
import time
import inspect
import ctypes



# 杀死daemon 守护进程的固定组合
def _async_raise(tid, exctype):
	"""raises the exception, performs cleanup if needed"""
	tid = ctypes.c_long(tid)
	if not inspect.isclass(exctype):
		exctype = type(exctype)
	res = ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, ctypes.py_object(exctype))
	if res == 0:
		raise ValueError("invalid thread id")
	elif res != 1:
		# """if it returns a number greater than one, you're in trouble,
		# and you should call it again with exc=NULL to revert the effect"""
		ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, None)
		raise SystemError("PyThreadState_SetAsyncExc failed")

def stop_thread(thread):
	_async_raise(thread.ident, SystemExit)



def pth_daemon_func():
	while True:
		print("Running background task...")
		time.sleep(2)

# 创建守护线程
pth_daemon = threading.Thread(target=pth_daemon_func)

# 设置为守护线程
pth_daemon.setDaemon(True)

# 启动守护线程
pth_daemon.start()



# 主线程继续执行
print("Main thread is running")
time.sleep(5)
print("Main thread finished")



# 强制杀死守护进程
stop_thread(pth_daemon)



