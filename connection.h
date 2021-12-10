/*
 * connection.h
 *
 *  Created on: 2021-11-24
 *      Author: @v-barros
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_
#include <stdlib.h>
// create a socket and connect to ADDRESS, return socket file descriptor
// terminate program on failure
int create_conn(char * ADDRESS, u_int16_t PORT);

void close_socket(int SOCKET_FD);

#endif // CONNECTION_H_