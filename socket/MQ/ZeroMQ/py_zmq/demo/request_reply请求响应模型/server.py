#coding=UTF-8


import zmq
import sys


context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")


while(True):
	try:
		print("server: wait for client ...")
		message = socket.recv()
		print("server: ", message.decode('utf-8'))
		socket.send(message)
	except Exception as e:
		print('server异常:',e)
		sys.exit()
