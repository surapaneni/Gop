#!/usr/bin/python
import socket
import sys
from time import sleep
try:
	HOST = sys.argv[1] 
	PORT = int(sys.argv[2])
except:
	HOST = "localhost"
	PORT = 8080

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((HOST,PORT))
flag = True
while flag:
	try:
		b = s.recv(255)
		s.send(b)
	except:
		b = None
	if b:
		try:
			b = int(b)
			if not b%10000:
				pass
				#print b
		except:
			pass
			#print b
		flag = True
	else:
		flag = False
