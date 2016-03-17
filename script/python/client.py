import socket
import time

#for md5
import hashlib
# for datalength
import struct,sys

def htons(num):
    return struct.pack('!H',num)
def htonl(num):
    return struct.pack('!I',num)

def md5(str):
    m = hashlib.md5()
    m.update(str)
    return m.hexdigest()

address = ('127.0.0.1',6000)
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)


try:
    s.connect(address)
except Exception,e:
    print "connect:%s" % e
    exit();

while True:
    data = raw_input("Please Input Data :")
    if len(data) > 0:
        try:
            s.send(md5(data)+htonl(len(data))+data)
        except Exception,e:
            print "send data error:%s" %e
        continue
    else:
        break

s.close()
