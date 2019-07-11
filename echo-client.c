// echo client. sends data to server and receives it back

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define PORT 13373
#define BUFSIZE 256

void error (char * msg) {
    perror(msg);
    exit (1);
}

void main (int argc, char *argv[]) {
    int clientSock, msgSize;
    struct sockaddr_in serverAddr;
    char echoBuf [BUFSIZE];
    
    // message to be sent to close a network connection
    const char quitMsg[5] = "quit";
    
    if ((clientSock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        error("ERROR(c) opening socket");
    }
    
    // set up server address/format
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // 0 = ANY, no need to change endian-ness
    memset(serverAddr.sin_zero, 0, strlen(serverAddr.sin_zero));
    
    // connect to server
    if (connect(clientSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        error("ERROR(c) on connect");
    }

    // report to server
    // this is where data such as the IP address could be taken from serverAddr struct
    printf("Welcome to the echo server! I will be your server today.\nSend any messages and have them returned back to you. Type \"quit\" to exit.\n");
    
    while (1) {
        // a tiny prompt + buffer (limits amount of data sent)
        // TODO: make this really big and make sends a smaller buffer
        printf("> ");
        fgets(echoBuf, BUFSIZE-1, stdin);
        
        // stop the program from getting newlines in fgets
        if (echoBuf[strlen(echoBuf)-1] == '\n') {
            echoBuf[strlen(echoBuf) - 1] = '\0';
        }
        
        // check if the quit message was sent
        if (strcmp(echoBuf, quitMsg) == 0) {
            printf("Closing connection\n");
            close(clientSock);
            break;
        }

        msgSize = strlen(echoBuf);
        
        // send that data to server
        if (send(clientSock, echoBuf, msgSize, 0) != msgSize) {
            error("ERROR on send");
        }
        
        // asking for data back
        // this is where the wait occurs in the run
        if ((msgSize = recv(clientSock, echoBuf, BUFSIZE, 0)) < 0) {
            error("ERROR on recv 2");
        }
        // report to server that data was received
        printf("We got: %s\n", echoBuf);
    }
}
