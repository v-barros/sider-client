/*
 * client.h
 *
 *  Created on: 2021-11-21
 *      Author: @v-barros
 */
#ifndef CLIENT_H_
#define CLIENT_H_
#include <stdlib.h>

typedef struct replyParser{
    char * buf;
    size_t len;
}replyParser;

typedef struct context {
    char *ipv4addr;
    char * wbuff; /* Write Buffer */
    replyParser* rParser; 
    int sockfd; /* socket file descriptor*/
    int bufflen;
    u_int16_t serverport;     
} context;

char * get_reply_str(context*cp);

int buffWrite(context *cp, int * done);

int buffRead(context *cp);

int parserFeed(replyParser * rp,const char *buff, ssize_t len);

context * contextInit(char * serveripv4addr, u_int16_t port);

void run(int sockfd);

int is_valid_get(char*c,int len);

int is_valid_set(int *keylen, int * valuelen, char* src, int srclen);

char * get(context * context,char * key);

char * set(context * context,char * key, char *value);

int validReply(replyParser * rp);


/*
    src must be a valid get command, ex:
    GET KEY
    len is the length of key
    dest is an empty string

    return sum of characters in dest
    
    $<command>$<param_len>${param}\r\n
    input: src=  GET key ; len = 7
    dest output: $0$3$key\r\n 
    return value = 10
*/
int encode_get(char *src,int len,char * dest);

#endif // CLIENT_H_