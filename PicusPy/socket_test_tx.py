import socket, sys, time
from picusData import *
import pickle

isMain = False


def sender(data):
    connGood = False
    tx = data.encode('utf-8')
    while True:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        print("waiting for receiver")
        while True:
            try:
                sock.connect(Picus.serverConn24)
                connGood = True
                break
            except socket.error as err:
                print(err)
                connGood = False
        while connGood:
            if not isMain:
                time.sleep(1.5)
            try:
                sock.send(pickle.dumps(time.clock()))
                if isMain:
                    print("TX: ", tx, "  :", time.clock())
            except socket.error as err:
                if err.errno == 10054 or err.errno == 54:
                    print("restarting")
                    try:
                        sock.close()
                    except socket.error: pass
                    connGood = False
                    break  # restart connection
                # print(err)
                pass
            time.sleep(0.02)


if __name__ == "__main__":
    isMain = True
    starttime = time.clock()
    sender("bb")

