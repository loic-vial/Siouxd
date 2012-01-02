#include <sys/types.h>
#include <sys/sochet.h>

import socket, sys, time, os
HOST = '192.168.1.53'
PORT = 5002

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
s.connect((HOST, PORT))
data = "Bonjour, je souhaite me connecter"
s.send(data.encode())
while 1 :
    data = s.recv(1024).decode()
    
    if not data :
            break
    print("client = ", data)
    
s.close()

