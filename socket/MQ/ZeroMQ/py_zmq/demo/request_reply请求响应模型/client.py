#coding=UTF-8


import zmq
import sys


context = zmq.Context()
print("client: Connecting to server...")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")


while(True):
	input_tmp = "hello server, i'm the client"
	socket.send(input_tmp.encode('utf-8'))

	message = socket.recv()
	print("client: ", message.decode('utf-8'))
