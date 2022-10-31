import socket

socket.setdefaulttimeout(30)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#srv_addr = ('172.24.44.109', 40810)
srv_addr = ('192.168.1.3', 40810)
sock.bind(srv_addr)
sock.listen()

while True:
    print("Waiting for connection")
    connection, client = sock.accept()
    #print(connection)

    try:
        print("Connected to client IP: {}".format(client))

        # Receive and print data 32 bytes at a time, as long as the client is sending something
        while True:
            data = connection.recv(32)
            print("Received data: {}".format(data))

            if not data:
                break

            print(f'sending back to ESP')
            connection.send(b'hello back from Gary\'s computer ;)\n');

    finally:
        connection.close()
        print('\n')