import socket
import msgpack
import random
from string import ascii_lowercase

#socket.setdefaulttimeout(30)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#srv_addr = ('172.24.44.109', 40810)
srv_addr = ('192.168.1.3', 40810)
sock.bind(srv_addr)
sock.listen()

byteseq = b'408A'

def randomizeCommands():
    randArr = [random.randint(0,255) for _ in range(3)] + [''.join([random.choice(ascii_lowercase) for _ in range(10)])]
    bites = msgpack.packb(randArr, use_bin_type=False)
    return bites, randArr

def calcFirstByte(data):
    newdata = b''
    ctr = 0
    idx = 0
    while (idx < len(data)-1):
        if data[idx] in [0xcc, 0xd0, 0xd9, 0xc4, 0xc7]: # 8-bits
            newdata += data[idx:idx+2]
            ctr += 1
            idx += 2
        elif data[idx] in [0xc5, 0xc8, 0xcd, 0xd1, 0xda]: # 16-bits
            newdata += data[idx:idx+3]
            ctr += 1
            idx += 3
        else: # other
            idx += 1
    
    return bytes([0x90 + ctr]), newdata

while True:
    print("Waiting for connection")
    connection, client = sock.accept()
    #print(connection)

    try:
        print("Connected to client IP: {}".format(client))
        
        # Receive and print data 32 bytes at a time, as long as the client is sending something
        while True:
            # RECEIVE
            data = connection.recv(1024)
            if data:
                b, d = calcFirstByte(data[:-4])
                print("Received data: {}".format(data))
                print("unpacking {}".format(b + d))
                unpacked = msgpack.unpackb(b + d, raw=True)
                print("Unpacked data: {}".format(unpacked))

            # SEND
            ### bites, randArr = randomizeCommands()
            ### print(f'sending data to ESP ---> {randArr}')
            ### connection.sendall(bites + b'\n');

    finally:
        connection.close()
        print('\n')