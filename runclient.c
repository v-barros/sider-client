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
    assert(argc == 3);
    int port = strtol(argv[2],NULL,10);
    context * scontext = contextInit(argv[1],port);

    // function for chat
    run(scontext->sockfd);
   
    // close the socket
    close_socket(scontext->sockfd);
}