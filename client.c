/*
 * client.c
 *
 *  Created on: 2021-11-21
 *      Author: @v-barros
 */
#include "client.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 110
#define maxkeylength 100

int read_command(char *buff);

void run(int sockfd)
{
    char buff[MAX];
    char aux[MAX];
    int len;
    for (;;) {
        memset(buff,0,sizeof(buff));
        memset(aux,0,sizeof(aux));
        printf("\n>");
        len = read_command(buff);
        len = is_valid_get(buff,len);
        if(len!=-1){
           // printf("buff \"%s\"",buff);
            len = encode_get(buff,len,aux);
           // printf("sending:\"%s\"%d\n",aux,len); // "$0$3$foo\r\n"
            write(sockfd, aux, len);
        }
        else{
            write(sockfd, buff, 14);
            printf("invalid command\n"); 
           // continue;
        }
            
        memset(buff,0,sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("\"%s\"", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}


//read stdin, put the string at str and return string length
//get key\n
//return 7, put "get key" in buff   
int read_command(char *buff){
    int n =0;
    while ((buff[n++] = getchar()) != '\n');
    buff[n-1]=buff[n];
    int k, v;
    printf("is_valid_set %d ", is_valid_set(&k,&v,buff,n-1));
    return n-1;
}

int encode_get(char *src,int keylen,char * dest){
    dest[0]='$';
    dest[1]='0';
    dest[2]='$';   
    int n = itostring(keylen,dest+3);
    dest[3+n]='$';
    memcpy(dest+4+n,src+4,keylen);
    dest[4+n+keylen]='\r';    
    dest[5+n+keylen]='\n';
    return 6 + n +keylen;
}

char * get(char * key){
    return NULL;
}

char * set(char * key, char *value){
    return NULL;

}

// check if input string is a valid get command
// return -1 if false
// return key length otherwise
// ex: c = "get key" len = 7
// return 3
// ex: c = "get key key2" len = 11
// return -1
int is_valid_get(char*c, int len){
    int i=0,aux =0;
    char * key;
    char * check = "get";
    if(len>maxkeylength+4)
        return -1;

    while (i<3){
        if(check[i]!=tolower(c[i]))
            return -1;
        i++;
    }
    if(c[i++]!=' ')
        return -1;
    key =c+4;
        
    aux = len - 4;

    i = 0;
    while(i<aux){
        if(*key+i == ' ')
            return -1;
        i++;
    }
    return aux;    
}


// set foo bar 
// srclen 11
int is_valid_set(int *keylen, int * valuelen, char* src, int srclen){
    int i=0,aux =0, tempklen;
    char * temp;
    char * check = "set";
    
    if(srclen>MAX)
        return -1;

    while (i<3){
        if(check[i]!=tolower(src[i]))
            return -1;
        i++;
    }
    if(src[i]!=' ')
        return -1;
    temp =src+4;

    i=0;

    while(temp[i]!=' '){ 
        if(i>=srclen-4) 
            return -1;
        i++;
    }
    if(!i)
        return -1;
    tempklen = i;

    i = 0;
    temp = src+4+tempklen;

    if(*temp!=' ')
        return -1;
    temp++;
    while(i<srclen-5-tempklen){
        if(*temp+i==' ') 
            return -1;
        i++;
    }
    if(!i)
        return -1;
    *keylen = tempklen;
    *valuelen = i;

    return 1;

}