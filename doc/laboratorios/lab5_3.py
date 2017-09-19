import socket

LED_TOGGLE_REQUEST = (0x79)
LED_SET_STATE = (0x7A)
LED_GET_STATE = (0x7B)
LED_STATE = (0x7C)

UDP_PORT = 8802
UDP_IP = "2804:7f4:3b80:3483:10d3:8421:b747:483c"
#UDP_IP = "fd50::203:3:3:3"
MESSAGE = "Sending UDP packet using python"
print "UDP target IP: ", UDP_IP
print "UDP target port: ", UDP_PORT
print "message: ", MESSAGE
sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
sock.sendto(LED_STATE, (UDP_IP, UDP_PORT))