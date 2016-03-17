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
                md5_str = data[0:32]
                data_len = ntohl(data[32:36])
                realdata = data[36:36+data_len]
                if md5(realdata) == md5_str:
                    print realdata
                else:
                    print "md5 error %s" % md5_str

                continue
            else:
                break
        self.request.close()

tcpServ = SocketServer.ThreadingTCPServer(ADDR,MyRequestHandler)
tcpServ.serve_forever()
