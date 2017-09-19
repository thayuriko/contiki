import socket
import struct
import random

HOST = ''
UDP_PORT = 8802

sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM) #UDP_PORT
sock.bind((HOST,UDP_PORT))

LED_TOGGLE_REQUEST = (0x79)
LED_SET_STATE = (0x7A)
LED_GET_STATE = (0x7B)
LED_STATE = (0x7C)

while True:
    data, addr = sock.recvfrom(1024) #BUFFER SIZE is 1024 BYTES
    #print "Received Message: ", data,"from: [", addr[0].strip(),"]:",addr[1]
    offset = 0
    op = struct.unpack_from(">B", data , offset)
    offset += struct.calcsize(">B")

    if op[0] == LED_TOGGLE_REQUEST:
        print "Received LED_TOGGLE_REQUEST, Send LED_TOGGLE to [", addr[0].strip(),"]:",addr[1]
        stt = random.randint(0,3)
        msg = struct.pack(">BB", LED_SET_STATE, stt)
        sock.sendto(msg, (addr[0].strip(), addr[1]))

    if op[0] == LED_STATE:
        state = struct.unpack_from(">B", data, offset)
        print "Received LED_STATE: ", state