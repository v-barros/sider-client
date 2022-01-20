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
    int port = (int)strtol(argv[2],NULL,10);
    context * scontext = contextInit(argv[1],port);
/*
    char * str = (char *)malloc(16);
    memcpy(str,"$1$3$3$foobar\r\n",15);
    scontext->wbuff = str;
    scontext->bufflen=15;

    printf("\n>%d",buffWrite(scontext,NULL));
    if(buffRead(scontext)){
        printf("\n>%s\n",get_reply_str(scontext));
        printf("\nvalid%d",validReply(scontext->rParser));
    }*/
    printf("\n\n get> %s\n",get(scontext,"foo"));
    // function for chat
   // run(scontext->sockfd);
    
    close_socket(scontext->sockfd);
}