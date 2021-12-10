/*
 * runclient.c
 *
 *  Created on: 2021-11-21
 *      Author: @v-barros
 */
#include "client.h"
#include "connection.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    assert(argc == 3);
    int port = strtol(argv[2],NULL,10);
    context * scontext = contextInit(argv[1],port);

    char * str = (char *)malloc(10);
    memcpy(str,"$0$3$foo\r\n",10);
    scontext->wbuff = str;
    scontext->bufflen=10;
    printf("\n>%d",buffWrite(scontext));
    // function for chat
   // run(scontext->sockfd);
    
    
    close_socket(scontext->sockfd);
}