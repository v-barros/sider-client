/*
 * connection.h
 *
 *  Created on: 2021-11-24
 *      Author: @v-barros
 */

// create a socket and connect to ADDRESS, return socket file descriptor
// terminate program on failure
int connect(char * ADDRESS);

void close_socket(int SOCKET_FD);