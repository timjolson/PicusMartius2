import socket, sys, time
from picusData import *
import pickle


def receiver(data):
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    server.bind(Picus.serverConn24)
    server.settimeout(Picus.localTimeout)
    server.listen(1)
    while True:
        print("waiting for transmitter")
        while True:
            try:
                connection, addr = server.accept()
                break
            except socket.timeout:
                pass
            except socket.error as err:
                print (err)
        while True:
            try:
                rx = connection.recv(1024)
            except socket.error as err:
                if err.errno == 10054 or err.errno == 54 or err.errno == 10061:
                    print("client lost")
                    break
            if rx:
                print("RX: ", time.clock() - pickle.loads(rx))
                connection.sendall("rx".encode('utf-8'))
            else:
                print("client closing")
                connection.close()
                break

if __name__ == "__main__":
    starttime = time.clock()
    receiver(0)

