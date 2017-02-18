# This script is designed to run the arduino on 192.168.1.100:5000 with host at 192.168.1.101



from socket import *
import time

number = 1

# Rover IP address is 192.168.1.20
while(True):
    address=( '192.168.1.20', 5000) #define server IP and port
    client_socket = socket(AF_INET, SOCK_DGRAM) #Set up the Socket
    client_socket.settimeout(1) #Only wait 1 second for a response

    number = number + 1
    data = str(number)

    client_socket.sendto( data.encode('utf-8'), address)

print("end")
