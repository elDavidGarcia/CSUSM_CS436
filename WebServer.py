#Echo server program
import socket
import sys

HOST = ''
PORT = 6666

#Create the socket
server_Socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print 'Socket was created.'

#Bind the socket
try:
    server_Socket.bind((HOST, PORT))
except socket.error, msg:
     print 'Falied to bind. \nError Code: ' + str(msg[0]) + ' \nMessage: ' + msg[1]
     sys.exit()
print 'Socket bind was successful to \nHOST: ' + str(HOST) + '\nPORT: ' + str(PORT)

#Make the socket listen
server_Socket.listen(1)      #One client 
print 'Socket now listening'

while True:

     #Establish the connection
     connectionSocket, addr = server_Socket.accept()
     print 'Connected with ' + addr[0] + ':' + str(addr[1])
     try:
         
         message = connectionSocket.recv(1024).decode()
         filename = message.split()[1] #Filename is second
         print 'File Name: ' + str(filename)
         f = open(filename, 'r')
         outputdata = f.read() #Load the data in the file to a variable
         
         #Send one HTTP header line into socket
         connectionSocket.send("""HTTP/1.0 OK
         Content-Type: text/html
         <html>
         Job Completed!
         </html>""".encode());

         #Send the content of the requested file to the client
         for i in range(0, len(outputdata)):
             connectionSocket.send(outputdata[i])

         #Close client Socket and break loop
         connectionSocket.close()
         break

    
     #Exception Handling 
     except IOError:
         print 'Error 404 File Not Found!'
         #Let client know file was not found
         connectionSocket.send('404 File Not Found!')

         #Close client socket and break loop
         connectionSocket.close()
         break

#Close server socket
server_Socket.close()
