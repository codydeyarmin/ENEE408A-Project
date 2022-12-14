import socket
import msgpack
import random
from string import ascii_lowercase
import time

#socket.setdefaulttimeout(30)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
#srv_addr = ('172.24.44.109', 40810)
srv_addr = ('192.168.1.3', 40810)
sock.bind(srv_addr)
sock.listen()

byteseq = b'408A'

def randomizeCommands():
    randArr = [random.randint(0,255) for _ in range(3)] + [''.join([random.choice(ascii_lowercase) for _ in range(10)])]
    bites = msgpack.packb(randArr, use_bin_type=False)
    return bites, randArr

while True:
    print("Waiting for connection")
    connection, client = sock.accept()
    #print(connection)

    try:
        print("Connected to client IP: {}".format(client))
        start = time.time()
        
        # Receive and print data 32 bytes at a time, as long as the client is sending something
        data = b''
        datacount = 0
        while True and (time.time() - start < 10):
            # RECEIVE
            data += connection.recv(1)
            if len(data) > 5:
                print(f'data = {data}')
            if len(data) > 4 and data[-4:] == b'408A':
                b, d = calcFirstByte(data[:-4])
                print("Received data: {}".format(data))
                #print("unpacking {}".format(b + d))
                #try:
                #    unpacked = msgpack.unpackb(b + d, raw=False)
                #    print("Unpacked data: {}\n".format(unpacked[-6:]))
                #except:
                #    print(f'ERROR --- msgpackerror\n')
                
                #datacount += 1
                data = b''

            # SEND
            ### bites, randArr = randomizeCommands()
            ### print(f'sending data to ESP ---> {randArr}')
            ### connection.sendall(bites + b'\n');
        #print(f'total messages received = {datacount}')

    finally:
        connection.close()
        print('\n')