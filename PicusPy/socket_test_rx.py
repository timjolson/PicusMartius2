import socket, sys, time
from picusData import *
import pickle

# receives and processes socket packets
def receiver(data):
    # start server
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # set socket for quick re-use
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # bind the server address to the server object
    server.bind(Picus.serverConn24)
    # set timeout on the sockets fairly low so things don't get stuck
    server.settimeout(Picus.localTimeout)
    # tell the server to listen for a certain number of connections
    server.listen(1)
    
    # run this all the time
    while True:
        print("waiting for transmitter")
        # wait for a connection
        while True:
            try:
                connection, addr = server.accept()
                # connection is good, bump to next loop
                break
            except socket.timeout:
                # connection timedout, stay in this loop
                pass
            except socket.error as err:
                # connection error, print and stay in this loop
                print (err)
        
        # wait for data
        while True:
            try:
                rx = connection.recv(1024)
                # try to receive a packet
            except socket.error as err:
                # if there is an error involving client socket closing
                if err.errno == 10054 or err.errno == 54 or err.errno == 10061:
                    print("client lost")
                    break
            
            # process packet
            if rx:
                # have received data and rx != 0
                print("RX: ", time.time() - pickle.loads(rx)) # print transmit and process time
                print (pickle.loads(rx))
                connection.sendall("rx".encode('utf-8')) # send an acknowledge if you want to
            else:
                # rx == 0 means client sent 0 as disconnect
                print("client closing")
                connection.close()
                break


# starts process clock
# starts receiver
if __name__ == "__main__":
    starttime = time.time()
    receiver(0)

