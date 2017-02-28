# This is to simplify UDP communication - David Ziliak

import socket
from socket import *
import time
from picusData import *
import pickle

class UDP_Library():

    def __init__(self, myAddr, address):
        self.address = address

        self.client_socket = socket(AF_INET, SOCK_DGRAM) #Set up the Socket
        try:
            self.client_socket.bind(myAddr) # this is me
        except error as err:
            if err.errno == 10049:
                raise Exception("\r\r\n\nAddress error, try setting machine static IP or updating myAddr in the script...")

        self.client_socket.settimeout(Picus.localTimeout) #Only wait .1 second for a response

    def sendDAT(self, message):
        try:
            self.client_socket.sendto(message, self.address)
        except:
            print("send dat fail")
            pass

    def sendTXT(self, message):
        try:
            self.client_socket.sendto(message.encode('utf-8'), self.address)
        except:
            print("send text fail")
            pass

    def receiveUDP(self):
        try:
            rx, addr = self.client_socket.recvfrom(1024)
            return rx
        except timeout:
            #print("timeout")
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


if __name__ == '__main__':
    start = time.time()
    last = 0
    myAddr = ("192.168.1.200", 7008)
    otherAddr = ("192.168.1.100", 6008)
    otherside = UDP_Library( myAddr, otherAddr)
    while True:
        #otherside.sendTXT("t")
        curr = time.time()
        if curr - last > 3:
            last = curr
            print("waiting")

        try:
            rx = otherside.receiveUDP()
            if rx:
                last = curr
                rx = pickle.loads(rx)
                print(rx.x)
        except:
            #print("no pickle")
            pass

        if curr - start > 30:
            print("time up")
            break