#coding=UTF-8

import multiprocessing



def sender(pipe_parent):
	# parent 父管道向子管道发送数据
	pipe_parent.send('hello, my child')

def receiver(pipe_child):
	# child 子管道接收父管道发来的数据
	message = pipe_child.recv()
	print(message)



if(__name__ == '__main__'):
	# 创建管道
	pipe_parent, pipe_child = multiprocessing.Pipe()

	# 创建两个子进程, 并且把管道传递给这两个子进程
	p1 = multiprocessing.Process(target=sender, args=(pipe_parent,))
	p2 = multiprocessing.Process(target=receiver, args=(pipe_child,))

	# 启动这两个子进程
	p1.start()
	p2.start()

	# 等待这两个子进程结束
	p1.join()
	p2.join()
