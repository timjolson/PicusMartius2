import time
from picuspy.comms import UDP_Library, tcp_rx, tcp_tx, settings, packets

port = settings.Picus.local_ports['L']

start = time.time()
last = start

manager = tcp_rx.start_local_receive(port)

while True:
    curr = time.time()
    rx = tcp_rx.local_receive(manager)
    if rx:
        last = curr
        if type(rx) == float:
            print(rx)
        elif type(rx) == packets.ControlStruct:
            print(rx['x'])

    if curr - last > 10:
        break
