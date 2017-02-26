# This is to simplify UDP communication - David Ziliak

import socket
from socket import *
import time
from picusData import *

class UDP_Library():
    freq = 20
    last = 0

    def __init__(self, myAddr, address):
        self.address = address

        self.client_socket = socket(AF_INET, SOCK_DGRAM) #Set up the Socket
        try:
            self.client_socket.bind(myAddr) # this is me
        except error as err:
            if err.errno == 10049:
                #print("\r\nAddress error, try setting machine static IP or updating myAddr in the script...\r\n")
                raise Exception("\r\r\n\nAddress error, try setting machine static IP or updating myAddr in the script...")

        self.client_socket.settimeout(0.1) #Only wait .1 second for a response

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
    myAddr = ("192.168.1.100", 5000)
    otherAddr = ("192.168.1.200", 5000)
    otherside = UDP_Library( myAddr, otherAddr)
    while True:
        otherside.sendUDP("test1234test9876")
        print("tx")
        time.sleep(0.5)
        try:
            print(otherside.receiveUDP())
        except:
            pass