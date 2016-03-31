#!/usr/bin/python
import sys

if(len(sys.argv) <= 1):
	print ("arg num is error")
	exit()

_list_conntent = []

fh = open("./test.conf",'rb')
for i in fh.readlines():
	_list_conntent.append(i)
fh.close()

for i in _list_conntent:
	if(i.find("ip") > 0):
		print i;
