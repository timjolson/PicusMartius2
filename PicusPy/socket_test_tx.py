import socket, sys, time
from picusData import *
import pickle

# flag for when the script is run directly
isMain = False

# send data
def sender(data):
    # flag for presence of connection
    connGood = False
    
    # encode data string to be smaller packet
    tx = data.encode('utf-8')
    
    # do this all the time
    while True:
        # make client socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # set socket for quick re-use
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        
        # wait for connection
        print("waiting for receiver")
        while True:
            try:
                sock.connect(Picus.serverConn24)
                # try to connect to server
                connGood = True
                # connection good if made it this far
                break
            except socket.error as err:
                # socket err, print and stay in this loop
                print(err)
                connGood = False
                
        # until connection has problem
        while connGood:
            # running directly, pause between tx
            if not isMain:
                time.sleep(1.5)
            
            # try sending data
            try:
                # pack the current time and send
                sock.send(pickle.dumps(time.clock()))
                
                # running directly, print what was transmitted
                if isMain:
                    print("TX: ", tx, "  :", time.clock())
                    
            # socket has an error
            except socket.error as err:
                # error involving server close
                if err.errno == 10054 or err.errno == 54:
                    print("restarting")
                    try:
                        # try to close the socket so we can restart it
                        sock.close()
                    except socket.error: pass # if socket did not exist on .close(), ignore error
                    connGood = False # flag connection has ended
                    break  # return to first loop
                
                pass # made it this far, stay in this loop
            
            # between restart, delay a while
            time.sleep(0.02)


# sets flag, starts process clock, sends data
if __name__ == "__main__":
    isMain = True
    starttime = time.clock()
    sender("bb")

