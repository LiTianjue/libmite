#!/usr/bin/python

import socket
import sys

ipaddr = "172.16.2.16"

address = (ipaddr,514)

level = 0
facility = 2

s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)


f= file("http.json", "r")

line = f.read()


msg = '<%d> : %s' % (level+facility,line)

s.sendto(msg,address)


f.close()
s.close()


