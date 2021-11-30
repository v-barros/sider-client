/*
 * runclient.c
 *
 *  Created on: 2021-11-21
 *      Author: @v-barros
 */
#include "client.h"
#include "connection.h"
#include <assert.h>

int main(int argc, char **argv)
{
    assert(argc == 2);

    int socket_fd = create_conn(argv[1]);
    // function for chat
    run(socket_fd);
   
    // close the socket
    close_socket(socket_fd);
}