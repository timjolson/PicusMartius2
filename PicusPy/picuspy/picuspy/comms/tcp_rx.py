import socket, sys, time
from picusData import *
import pickle

# flag for when the script is run directly
isMain = False

def start_local_receive(port):
    # start server
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # set socket for quick re-use
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # bind the server address to the server object
    #server.bind((Picus.conn24[0], port))
    server.bind(('localhost', port))
    # set timeout on the sockets fairly low so things don't get stuck
    server.settimeout(Picus.localTimeout)

    return server

def local_receive(server):
    yes_return = False
    server.listen(1)

    try:
        connection, addr = server.accept()
        # connection is good, bump to next loop
    except socket.timeout:
        # connection timedout, stay in this loop
        pass
    except socket.error as err:
        # connection error, print and stay in this loop
        print(err)
    else:
        try:
            rx = connection.recv(1024)
            # try to receive a packet
        except socket.error as err:
            # if there is an error involving client socket closing
            if err.errno == 10054 or err.errno == 54 or err.errno == 10061:
                print("client lost")

        # process packet
        if rx:
            # have received data and rx != 0
            if type(rx) == bytes:
                rx = pickle.loads(rx)
                yes_return = True
            else:
                print("RX type invalid ", rx)

        else:
            # rx == 0 means client sent 0 as disconnect
            print("client closing")

    try:
        connection.close()
    except: pass

    if yes_return: return rx
    else: return None

# # receives and processes socket packets
# def receiver(data=""):
#     # start server
#     server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#     # set socket for quick re-use
#     server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
#
#     # bind the server address to the server object
#     server.bind((Martius.conn24[0], 8000))
#     # set timeout on the sockets fairly low so things don't get stuck
#     server.settimeout(Picus.localTimeout)
#     # tell the server to listen for a certain number of connections
#     server.listen(1)
#
#     # run this all the time
#     while True:
#         if isMain:
#             print("waiting for transmitter")
#         # wait for a connection
#         while True:
#             try:
#                 connection, addr = server.accept()
#                 # connection is good, bump to next loop
#                 break
#             except socket.timeout:
#                 # connection timedout, stay in this loop
#                 pass
#             except socket.error as err:
#                 # connection error, print and stay in this loop
#                 print (err)
#
#         # wait for data
#         while True:
#             try:
#                 rx = connection.recv(1024)
#                 # try to receive a packet
#             except socket.error as err:
#                 # if there is an error involving client socket closing
#                 if err.errno == 10054 or err.errno == 54 or err.errno == 10061:
#                     print("client lost")
#                     break
#
#             # process packet
#             if rx:
#                 # have received data and rx != 0
#                 if type(rx) == bytes:
#                     rx = pickle.loads(rx)
#
#                 if isMain:
#                     if type(rx) == str:
#                         print("RX: ", rx)
#                     elif type(rx) == float:
#                         print("RX: ", time.time() - rx) # print transmit and process time
#                 # connection.sendall("rx".encode('utf-8')) # send an acknowledge if you want to
#             else:
#                 # rx == 0 means client sent 0 as disconnect
#                 print("client closing")
#                 connection.close()
#                 break


# starts process clock
# starts receiver
if __name__ == "__main__":
    isMain = True
    start = time.time()
    serv = start_local_receive(9000)  # setup port

    # watch for data on setup port
    while time.time() - start < 40:
        a = local_receive(serv)
        if a: print(a)

    exit(0)