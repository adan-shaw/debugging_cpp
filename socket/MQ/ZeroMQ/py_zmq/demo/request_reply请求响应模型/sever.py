import zmq
import sys


context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")


while(True):
	try:
		print("wait for client ...")
		message = socket.recv()
		print("message from client:", message.decode('utf-8'))
		socket.send(message)
	except Exception as e:
		print('异常:',e)
		sys.exit()
