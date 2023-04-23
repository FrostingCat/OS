#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 

#define MAXPENDING 5
#define RCVBUFSIZE 32

void DieWithError(char *errorMessage);
void HandleTCPClient(int clntSocket);

int main(int argc, char *argv[]) {
    int sendSock, getSock;                    /* Socket descriptor for server */
    int getSocket, sendSocket;                    /* Socket descriptor for client */
    struct sockaddr_in echoSendAddr, echoGetAddr; /* Local address */
    struct sockaddr_in echoClntGetAddr, echoClntSendAddr; /* Client address */
    unsigned short echoGetPort, echoSendPort;     /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */

    if (argc != 3)     /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage:  %s <Sender Port>  <Getter Port>\n", argv[0]);
        exit(1);
    }

    echoSendPort = atoi(argv[1]); /* First arg:  local sender port */
    echoGetPort = atoi(argv[2]);  /* First arg:  local getter port */

    /* Create socket for incoming connections */
    if ((sendSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    if ((getSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct local address structure */
    memset(&echoSendAddr, 0, sizeof(echoSendAddr));   /* Zero out structure */
    echoSendAddr.sin_family = AF_INET;                /* Internet address family */
    echoSendAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoSendAddr.sin_port = htons(echoSendPort);      /* Local port */

    memset(&echoGetAddr, 0, sizeof(echoGetAddr));   /* Zero out structure */
    echoGetAddr.sin_family = AF_INET;                /* Internet address family */
    echoGetAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoGetAddr.sin_port = htons(echoGetPort);      /* Local port */

    /* Bind to the local address */
    if (bind(getSock, (struct sockaddr *) &echoGetAddr, sizeof(echoGetAddr)) < 0)
        DieWithError("bind1() failed");
    if (bind(sendSock, (struct sockaddr *) &echoSendAddr, sizeof(echoSendAddr)) < 0)
        DieWithError("bind2() failed");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(getSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    char echoBuffer[RCVBUFSIZE];
    int recvMsgSize;

    if ((getSocket = accept(getSock, (struct sockaddr *) &echoClntGetAddr,
                               &clntLen)) < 0)
            DieWithError("accept() failed");
    if ((sendSocket = accept(sendSock, (struct sockaddr *) &echoClntSendAddr,
                               &clntLen)) < 0)
            DieWithError("accept() failed");
    if ((recvMsgSize = recv(getSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");

    while (strcmp(echoBuffer, "The End")) {
        echoBuffer[recvMsgSize] = '\0';
        printf("%s", echoBuffer);
        if (send(sendSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
            DieWithError("send() failed");
        clntLen = sizeof(echoClntGetAddr);
        if ((getSocket = accept(getSock, (struct sockaddr *) &echoClntGetAddr,
                               &clntLen)) < 0)
            DieWithError("accept() failed");

        HandleTCPClient(getSocket);
    }
    close(sendSocket);
    close(getSocket);
}

