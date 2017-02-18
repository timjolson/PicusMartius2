import threading, time
from queue import Queue
import pickle
from socket_test_tx import *
from socket_test_rx import *

print_lock = threading.Lock()
q = Queue()
threads = [
    ("sender", "aa"),
    ("receiver", ""),
    ]


def threader():
# http://stackoverflow.com/questions/3061/calling-a-function-of-a-module-from-a-string-with-the-functions-name-in-python
    while True:
        proc, arg = q.get()
        globals()[proc](arg)
        q.task_done()


if __name__ == "__main__":
    for x in threads:
        starttime = time.clock()
        t = threading.Thread(target = threader)
        t.daemon = True  # full application will close when all that's left running is daemons
        t.start()

    for x in threads:
        q.put(x)

    q.join()
