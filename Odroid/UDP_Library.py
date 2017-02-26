# This is to simplify UDP communication - David Ziliak

import socket
from socket import *
import time

class UDP_Library():
    freq = 20
    last = 0

    def __init__(self, myAddr, address, port):
        self.address = (address, port)
        self.port = port

        self.client_socket = socket(AF_INET, SOCK_DGRAM) #Set up the Socket
        self.client_socket.bind(myAddr) # this is me
        self.client_socket.settimeout(0.1) #Only wait 1 second for a response

    def sendUDP(self, message):
        try:
            self.client_socket.sendto(message.encode('utf-8'), self.address)
        except:
            #TODO: print when no connection
            pass

    def receiveUDP(self):
        if time.clock() - self.last > (1/self.freq):
            self.last = time.clock()
            try:
                #print("b4")
                rxText, addr = self.client_socket.recvfrom(1024)
                print("rx:"+rxText.decode('utf-8'))
                return rxText.decode('utf-8')
            except timeout:
                print("timeout")
                return 0
            except herror:
                print("herror")
                return 0
            except gaierror:
                print("gaierror")
                return 0
            except error as msg:
                print("error:", msg)
                return 0
        else:
            #self.last = time.clock()
            return 0


if __name__ == '__main__':
    myAddr
    otherAddr = "192.168.1.100"
    port = 5000
    otherside = UDP_Library( myAddr, otherAddr, port)
    otherside.sendUDP(raw_input("message:"))