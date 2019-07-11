// echo server. sends back anything it gets
// only handles one client at a time

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define PORT 13373
#define BUFSIZE 256

void setLocalSocAddr (struct sockaddr_in *serverAddr, int port) {
    (*serverAddr).sin_family = AF_INET;
    (*serverAddr).sin_port = htons(port);
    (*serverAddr).sin_family = 0; // 0 = ANY, no need to change endian-ness
    memset((*serverAddr).sin_zero, 0, strlen((*serverAddr).sin_zero));
}

void error (char * msg) {
    perror(msg);
    exit (1);
}

void main (int argc, char *argv[]) {
    int sockid, clientLen, clientSock, msgSize;
    struct sockaddr_in serverAddr, clientAddr;
    char echoBuf [BUFSIZE];
    
    // create the socket
    sockid = socket(PF_INET, SOCK_STREAM, 0);
    
    if (sockid < 0) {
        error("ERROR opening socket");
    }
    
    // set up server address/format
    setLocalSocAddr(&serverAddr, PORT);
    
    // bind to network addr and listen on port
    // might break if IP/port is already in TIME_WAIT state (check netstat -plantu)
    if (bind(sockid, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        error("ERROR on bind");
    }
    if (listen(sockid, 10) < 0) {
        error("ERROR on listen");
    }
    
    // complete! report to server side
    printf("Server has been successfully initialized (socket, bind, listen)\n");
    
    // start a loop of acccepting client connections and then receiving messages
    while (1) {
        clientLen = sizeof(clientAddr);
        
        if ((clientSock = accept(sockid, (struct sockaddr *)&clientAddr, &clientLen)) < 0) {
            error("ERROR on accept");
        }
        // report to server
        // this is where data such as the IP address could be taken from clientAddr struct
        printf("We have accepted a client\n");
        
        do {
            // asking for data
            // this is where the wait occurs in the run
            if ((msgSize = recv(clientSock, echoBuf, BUFSIZE, 0)) < 0) {
                error("ERROR on recv 2");
            }
            // null terminating rather than clearing buffer every time
            echoBuf[msgSize] = '\0';
            
            // report to server that data was received
            printf("We got some good data from client: %s\n", echoBuf);
            
            // send that data back to cliesont
            if (send(clientSock, echoBuf, msgSize, 0) != msgSize) {
                error("ERROR on send");
            }
        } while (msgSize > 0); // is 0 on disconnect
        
        // end connection, allows for new one
        printf("Closing conection with a client\n");
        close(clientSock);
    }
}
