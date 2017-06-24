# UDP_Library.py
# UDP_Library class runs UDP communication between two network nodes
# Written by Tim Olson - timjolson@user.noreplay.github.com
#
# Example at bottom is specific to Picus Martius testing.
# Will print x value from a received ControlStruct object (defined in picusData module)
import socket
from socket import *
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
            self.client_socket.sendto(pickle.dumps(message), self.address)
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
            try:
                return pickle.loads(rx)
            except:
                return None
        except timeout:
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
    import time
    from picusData import *
    
    start = time.time()
    last = 0
    myAddr = Picus.conn24
    otherAddr = Martius.conn24
    otherside = UDP_Library( myAddr, otherAddr)
    while True:
        #otherside.sendTXT("t")
        curr = time.time()
        if curr - last > 3:
            last = curr
            print("waiting")

        rx = otherside.receiveUDP()
        if rx:
            last = curr
            print(rx.x)

        if curr - start > 30:
            print("time up")
            break

