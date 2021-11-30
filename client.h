/*
 * client.h
 *
 *  Created on: 2021-11-21
 *      Author: @v-barros
 */
#ifndef CLIENT_H_
#define CLIENT_H_

void run(int sockfd);

int is_valid_get(char*c,int len);

int is_valid_set(char*c,int len);

char * get(char * key);

char * set(char * key, char *value);


/*
    $<command>$<param_len>{param}\r\n
    input: src=  GET key
    output: $0$3key\r\n
    src must be a valid get command, ex:
    GET KEY
    len is the length of key
    dest is an empty array
*/
int encode_get(char *src,int len,char * dest);

#endif // CLIENT_H_