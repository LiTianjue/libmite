import SocketServer
import hashlib

import struct 


def ntohl(data):
    return struct.unpack('!I',data)[0]

def md5(str):
    m = hashlib.md5()
    m.update(str)
    return m.hexdigest()


HOST = ''
PORT = 6000
ADDR = (HOST,PORT)
class MyRequestHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        while True:
            data =self.request.recv(512)
            if(len(data) > 0):
				print data
				self.request.send(data)
            else:
                break
        self.request.close()

tcpServ = SocketServer.ThreadingTCPServer(ADDR,MyRequestHandler)
tcpServ.serve_forever()
