#coding=UTF-8


import zmq


context = zmq.Context()
receive = context.socket(zmq.PULL)
receive.connect('tcp://127.0.0.1:5557')
sender = context.socket(zmq.PUSH)
sender.connect('tcp://127.0.0.1:5558')


while(True):
	data = receive.recv()
	print("worker: 正在转发...")
	sender.send(data)
