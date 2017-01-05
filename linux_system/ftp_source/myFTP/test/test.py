#!/usr/bin/python
# -*- coding: utf-8 -*-

import socket
import time
import random
import os
import re

########################################
# you should set the right values for this part


serverAddress = '127.0.0.1'
serverPort = 21
# foramt:
# 'name' : 'password'
users = { 'can':'123',
		'test':'test'}

clientAddress = '127.0.0.1'


########################################







size = 2 ** 11
socket.setdefaulttimeout(10)

def port(controlSocket):
	clientPort = random.randint(10000, 20000)

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind( (clientAddress, clientPort))
	s.listen(1)

	command = "PORT " + clientAddress.replace('.',',') + ',' + \
			   str(clientPort/256) + ',' + str(clientPort%256)
	controlSocket.send(command + '\r\n')
	print controlSocket.recv(size)
	return s


def pasv(controlSocket):
	controlSocket.send('PASV')
	data = controlSocket.recv(size)
	print data
	pattern = '([0-9 ]+),([0-9 ]+),([0-9 ]+),([0-9 ]+),([0-9 ]+),([0-9 ]+)'
	match = re.findall(pattern, data)
	if match:
		match = match[0]
		ipString = match[0] + '.' + match[1] + '.' + match[2] + '.' + match[3]
		port = int(match[4]) * 256 + int(match[5])
		print 'remote:', ipString, port
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		s.connect( (ipString, port))
		return s
	else:
		print 'PASV Fail!!'

def summary(title, flag):
	print '%-25s %s' % (title, 'OK' if flag else 'Fail')	


if __name__ == '__main__' :
	sockets = {}
	users['doesnotexist'] = 'hehe'

	print '########### authentication test ###########'
	for name in users:
		print 'name:', name, 'password:', users[name]

		try:
			s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			s.connect( (serverAddress, serverPort) )
			sockets[name] = s
			print 'using socket', s.getsockname()
		except:
			print "can't connect to server, double check address and port"

	print '------------------------------------'
	
# authentication
	authenticationFlag = True
	for name in sockets:
		s = sockets[name]
		s.recv(size)
		s.send('USER ' + name)
		s.recv(size)
		s.send('PASS ' + users[name])
		data = s.recv(size)
		print name, '[', data[0:-2], ']'
		
		if not data:
			data = ' '
		if data[0] != '2' and name != 'doesnotexist':
			authenticationFlag = False
		if data[0] == '2' and name == 'doesnotexist':
			authenticationFlag = False

	del users['doesnotexist']
	del sockets['doesnotexist']

	print '########### change to binary mode(optional) ###########'
	binaryModeFlag = True
	for name in sockets:
		s = sockets[name]
		s.send('TYPE I')
		data = s.recv(size)
		print name, '[', data[0:-2], ']'
		if not data:
			binaryModeFlag = False
			break
		if data[0] != '2':
			binaryModeFlag = False


	print '########### pwd, mkdir, cwd, rename ###########'
	for name in sockets:
		print ''
		print 'User: ', name
		print '------------------------------------'
		
		s = sockets[name]

		s.send('PWD')
		print 'try: pwd'
		print s.recv(size)[0:-2]

		s.send('MKD ' + name + 'looooogsuffix')
		print 'try: mkdir ', name + 'looooogsuffix'
		print s.recv(size)[0:-2]

		s.send('CWD ' + name + 'looooogsuffix')
		print 'try: cd ', name + 'looooogsuffix'
		print s.recv(size)[0:-2]

		s.send('CWD ..')
		print 'try: cd ..'
		print s.recv(size)[0:-2]

		s.send('PWD')
		print 'try: pwd'
		print s.recv(size)[0:-2]

		print 'try: rename ', name + 'looooogsuffix', name + 'shortsuffix'
		s.send('RNFR '+ name + 'looooogsuffix')
		print s.recv(size)[0:-2]
		s.send('RNTO '+ name + 'shortsuffix')
		print s.recv(size)[0:-2]


	print '########### active mode LIST ###########'
	listFlag = True
# port & list
	for name in sockets:
		s = port(sockets[name])
		time.sleep(1)	
		sockets[name].send('LIST\r\n')
		try:
			connect, address = s.accept()
			time.sleep(1)
			data = connect.recv(size)
			print 'LIST format:'
			print '------------------------------------'		
			print data
			print '------------------------------------'
			print sockets[name].recv(size)
			connect.close()
		except:
			print 'Fail!!'
			listFlag = False
		s.close()
		break		# test once

	print '########### active mode STOR ###########'
	print 'upload file1.pdf as test.pdf'
	fileSize = os.path.getsize('file1.pdf')
	totalTraffic = 0	# Byte
# port & stor
	activeStoreFlag = True
	for name in sockets:
		s = port(sockets[name])
		time.sleep(1)	# bug reminder
		sockets[name].send('STOR test.pdf')
		try:
			connect, address = s.accept()
			testFile = file('file1.pdf')
			startTime = time.time()
			while True:
				data = testFile.read(size)
				if(data):
					connect.send(data)
				else:
					break
			print sockets[name].recv(size)
			testFile.close()
			connect.close()
			print sockets[name].recv(size)
			endTime = time.time()
			print 'average speed:', fileSize/(endTime - startTime)/1024,'kB/s'
			totalTraffic += fileSize
			print 'total traffic:', totalTraffic, 'Bytes'
		except:
			print 'Fail!!'
			activeStoreFlag = False
		s.close()
		break		# test once 
		

	print '########### active mode RETR ###########'
	print 'download test.pdf as file2.pdf'
# port & retr
	activeRetrieveFlag = True
	for name in sockets:
		s = port(sockets[name])
		time.sleep(1)
		sockets[name].send('RETR test.pdf')
		try:
			connect, address = s.accept()
			testFile = open('file2.pdf', 'w')
			startTime = time.time()
			while True:
				data = connect.recv(size)
				if(data):
					testFile.write(data)
				else:
					break
			print sockets[name].recv(size)
			testFile.close()
			connect.close()
			endTime = time.time()
			print 'average speed:', fileSize/(endTime - startTime)/1024,'kB/s'
			totalTraffic += fileSize
			print 'total traffic:', totalTraffic, 'Bytes'
		except:
			print 'Fail!!'
			activeRetrieveFlag = False
		s.close()
		break		# test once



	print '########### passive mode STOR ###########'
	print 'upload file1.pdf as test2.pdf'
# pasv & stor
	passiveStoreFlag = True
	for name in	sockets:
		try:
			s = pasv(sockets[name])
			time.sleep(1)
			sockets[name].send('STOR test2.pdf')
			testFile = file('file1.pdf')
			startTime = time.time()
			while True:
				data = testFile.read(size)
				if(data):
					s.send(data)
				else:
					break
			print sockets[name].recv(size)
			testFile.close()
			s.close()
			print sockets[name].recv(size)
			endTime = time.time()
			print 'average speed:', fileSize/(endTime - startTime)/1024,'kB/s'
			totalTraffic += fileSize
			print 'total traffic:', totalTraffic, 'Bytes'		  
		except:
			print 'Fail!!'
			passiveStoreFlag = False	  
		break	# test once


	print '########### passive mode RETR ###########'
	print 'download test2.pdf as file3.pdf'
# pasv & retr
	passiveRetrieveFlag = True
	for name in sockets:
		try:
			s = pasv(sockets[name])
			time.sleep(1)
			sockets[name].send('RETR test2.pdf')
			testFile = open('file3.pdf', 'w')
			startTime = time.time()
			while True:
				data = s.recv(size)
				if(data):
					testFile.write(data)
				else:
					break
			print sockets[name].recv(size)
			testFile.close()
			s.close()
			endTime = time.time()
			print 'average speed:', fileSize/(endTime - startTime)/1024,'kB/s'
			totalTraffic += fileSize
			print 'total traffic:', totalTraffic, 'Bytes' 
		except:
			print 'Fail!!'
			passiveRetrieveFlag = False
		break	# test once
		

	print '########### delete files ###########'
# delete file1, file2 in remote
	deleteFlag = True
	for name in sockets:
		s = sockets[name]

		s.send('DELE test.pdf')
		print 'try: delete test.pdf'
		data = s.recv(size)
		print data[0:-2]
		if data[0] != '2' and activeStoreFlag:
			deleteFlag = False

		s.send('DELE test2.pdf')
		print 'try: delete test2.pdf'
		data = s.recv(size)
		print data[0:-2]
		if data[0] != '2' and passiveStoreFlag:
			deleteFlag = False

		s.send('DELE test3.pdf')
		print 'try: delete test3.pdf'
		data = s.recv(size)
		print data[0:-2]
		if data[0] == '2':
			deleteFlag = False
		break


	print '########### files compare ###########'	
# file comapre
	fileCompareFlag = True

	print 'file1 is the original file'
	print 'file2 is the file upload then download in active mode'
	print 'file3 is the file upload then download in passive mode'
	print '------------------------------------'	

	file1 = file('file1.pdf').readlines()

	file2 = None
	file3 = None
	try:
		file2 = file('file2.pdf').readlines()
	except:
		print 'file2 not exist, server may not support Active Mode'

	try:
		file3 = file('file3.pdf').readlines()
	except:
		print 'file3 not exist, server may not support Passive Mode'	

	if file1 == file2:
		print 'file1 == file2'
	else:
		print 'file1 != file2'
		if activeStoreFlag:
			fileCompareFlag = False

	if file1 == file3:
		print 'file1 == file3'
	else:
		print 'file1 != file3'
		if passiveStoreFlag:
			fileCompareFlag = False


	print ''
	print '########### Summary ###########'

	summary('authentication', authenticationFlag)
	summary('binary mode(optional)', binaryModeFlag)
	summary('LIST', listFlag)
	summary('active mode upload', activeStoreFlag)
	summary('active mode download', activeRetrieveFlag)
	summary('passive mode upload', passiveStoreFlag)
	summary('passive mode download', passiveRetrieveFlag)
	summary('delete file', deleteFlag)
	summary('file comapre', fileCompareFlag)
