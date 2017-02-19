# This script is designed to run the arduino on 192.168.1.100:5000 with host at 192.168.1.101



##from socket import *
##import time
##
##number = 1
##
##while(True):
##    address=( '192.168.1.100', 5000) #define server IP and port
##    client_socket = socket(AF_INET, SOCK_DGRAM) #Set up the Socket
##    client_socket.settimeout(1) #Only wait 1 second for a response
##
##    number = number + 1
##    data = str(number)
##
##    client_socket.sendto( data.encode('utf-8'), address)
##
##print("end")

import socket

# Change IP address to radio
UDP_IP = "127.0.0.1"
UDP_PORT = 5005
MESSAGE = "Hello, World!"

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
print "message:", MESSAGE

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
