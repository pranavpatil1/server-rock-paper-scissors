# server-rock-paper-scissors
This is a server implementation of rock paper scissors using sockets. Two players connect to the server at port 13373 with netcat, choose rock/paper/scissors, and the server sends back who won.

To set up server:
 - gcc server.c -o server
 - ./server

Leave that command running in one terminal tab.

To connect via clients:
 - nc SERVER_IP 13373
 
 For example, nc localhost 13373
