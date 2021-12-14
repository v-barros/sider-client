/*
 * connection.c
 *
 *  Created on: 2021-11-24
 *      Author: @v-barros
 */
#include "connection.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define SA struct sockaddr

int create_conn(char * address, u_int16_t port){
    int sockfd,connfd;
    struct sockaddr_in servaddr;
   
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    
    memset(&servaddr,0,sizeof(servaddr));
      
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(address);
    servaddr.sin_port = htons(port);
   
    // connect the client socket to server socket
    if (connect(sockfd,(SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
    return sockfd;
}

void close_socket(int SOCKET_FD){
    close(SOCKET_FD);
}