import socket
import msgpack
import random
from string import ascii_lowercase
import time

#socket.setdefaulttimeout(2)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
#srv_addr = ('172.24.44.109', 40810)
srv_addr = ('192.168.1.3', 40810)
sock.bind(srv_addr)
sock.listen()

byteseq = b'408A'

def randomizeCommands():
    randArr = [random.randint(0,1024) for _ in range(5)] # + [''.join([random.choice(ascii_lowercase) for _ in range(10)])]
    bites = msgpack.packb(randArr, use_bin_type=True)
    return bites, randArr

# TODO - 0xcc does not get added sometimes because arduino msgpack implementation is POS, so look for this here 
def calcFirstByte(data):
    newdata = b''
    ctr = 0
    idx = 0
    while (idx < len(data)-1):
        if data[idx] in [0xcc, 0xd0]: # 8-bits
            newdata += data[idx:idx+2]
            ctr += 1
            idx += 2
        elif data[idx] in [0xcd, 0xd1]: # 16-bits
            newdata += data[idx:idx+3]
            ctr += 1
            idx += 3
        else: # other
            newdata += b'\xcc' + data[idx:idx+1]
            idx += 1
            ctr += 1
    
    return bytes([0x90 + ctr]), newdata

while True:
    print("Waiting for connection")
    connection, client = sock.accept()
    #connection.settimeout(20)
    #print(connection)

    try:
        print("Connected to client IP: {}".format(client))
        start = time.time()
        
        # Receive and print data 32 bytes at a time, as long as the client is sending something
        data = b''
        datacount = 0
        sendit = True
        while True:# and (time.time() - start < 10):
            # SEND
            bites, randArr = randomizeCommands()
            print(f'\nsending data to ESP ---> {randArr}')
            connection.sendall(bites)
            #sendit = False
            #continue
            
            # RECEIVE
            data += connection.recv(1024)
            print(f'data = {data}')
            if len(data) > 4 and data[-4:] == b'408A':
                data = b'\x96' + data.split(b'\x96')[-1]
                #b, d = calcFirstByte(data[:-4])
                print("Received data: {}".format(data))
                #print("unpacking {}".format(b + d))
                print("unpacking {}".format(data[:-4]))
                try:
                    unpacked = msgpack.unpackb(data[:-4], raw=False)
                    #print("Unpacked data: {}\n".format(unpacked[-6:]))
                    print("Unpacked data: {}\n".format(unpacked))
                except:
                    print(f'ERROR --- msgpackerror\n')
                
                datacount += 1
                data = b''
                #sendit = True
            
            #time.sleep(1)
            
        print(f'total messages received = {datacount}')

    finally:
        connection.close()
        print('\n')