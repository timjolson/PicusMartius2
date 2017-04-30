import socket, sys, time
from picusData import *
import pickle

# flag for when the script is run directly
isMain = False

def local_send(port, data="",tried=True):
    # make client socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # set socket for quick re-use
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    sock.settimeout(Picus.localTimeout)

    try:
        sock.connect((Picus.conn24[0], port))
    except socket.error as err:
        if err.errno == 10054 or err.errno == 54 or err.errno == 10061 or err.errno == 111:
            pass
        else:
            # socket err, print
            print(err)
    else:
        try:
            sock.send(pickle.dumps(data,2))
        # socket has an error
        except socket.error as err:
            # error involving server close
            if err.errno == 10054 or err.errno == 54 or err.errno == 10061:
                if isMain: print("no receiver")
                try:
                    # try to close the socket so we can restart it
                    sock.close()
                except socket.error:
                    pass  # if socket did not exist on .close(), ignore error
        else:
            if not tried:
                sendTCP(port, data, tried=1)
            else:
                if isMain: print("no receiver")
                pass

# # send data
# def sender(data=""):
#     # flag for presence of connection
#     connGood = False
#
#     # do this all the time
#     while True:
#         # make client socket
#         sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#         # set socket for quick re-use
#         sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
#
#         # wait for connection
#         print("waiting for receiver")
#         while True:
#             try:
#                 sock.connect((Martius.conn24[0], 8000))
#                 # try to connect to server
#                 connGood = True
#                 # connection good if made it this far
#                 break
#             except socket.error as err:
#                 connGood = False
#                 if err.errno == 10054 or err.errno == 54 or err.errno == 10061:
#                     pass
#                 else:
#                     # socket err, print and stay in this loop
#                     print(err)
#
#         # until connection has problem
#         while connGood:
#             # running directly, pause between tx
#             # if isMain:
#             #     time.sleep(1.5)
#
#             # try sending data
#             try:
#                 # running directly, print what was transmitted
#                 if isMain and data=="":
#                     print("TX: ", time.time())
#                     sock.send(pickle.dumps(time.time()))
#                 elif isMain:
#                     print("TX: ", data)
#                     sock.send(pickle.dumps(data))
#                 else:
#                     sock.send(pickle.dumps(data))
#
#             # socket has an error
#             except socket.error as err:
#                 # error involving server close
#                 if err.errno == 10054 or err.errno == 54 or err.errno == 10061:
#                     print("lost receiver")
#                     try:
#                         # try to close the socket so we can restart it
#                         sock.close()
#                     except socket.error: pass # if socket did not exist on .close(), ignore error
#                     connGood = False # flag connection has ended
#                     break  # return to first loop
#
#                 pass # made it this far, stay in this loop
#
#             # between restart, delay a while
#             time.sleep(0.01)


# sets flag, starts process clock, sends data
if __name__ == "__main__":
    isMain = True
    start = time.time()

    # send data over port
    data = 123 # can be text or whatever
    while time.time() - start < 40:
        local_send(9000, data)
        time.sleep(0.5)

    exit(0)