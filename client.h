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

int is_valid_set(int *keylen, int * valuelen, char* src, int srclen);

char * get(char * key);

char * set(char * key, char *value);


/*
   

    src must be a valid get command, ex:
    GET KEY
    len is the length of key
    dest is an empty array

    return sum of characters in dest
    
    $<command>$<param_len>${param}\r\n
    input: src=  GET key ; len = 7
    dest output: $0$3$key\r\n 
    return value = 10
*/
int encode_get(char *src,int len,char * dest);

#endif // CLIENT_H_