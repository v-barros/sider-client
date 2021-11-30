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
        bzero(buff, sizeof(buff));
        bzero(aux,sizeof(aux));
        printf("\n>");
        len = read_command(buff);
        len = is_valid_get(buff,len);
        if(len!=-1){
            encode_get(buff,len,aux);
            printf("sending:\"%s\"%d\n",aux,len+4);
            write(sockfd, buff, len+4);
        }
        else{
            printf("invalid command\n"); 
            continue;
        }
            
        bzero(buff, sizeof(buff));
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
    return n-1;
}

int encode_get(char *src,int len,char * dest){
    dest[0]='$';
    dest[1]='0';
    dest[2]='$';   
    int n = itostring(len,dest+3);
    dest[3+n]='$';
    memcpy(dest+4+n,src,len);
   // dest[4+n+len]='\0';    
  //  dest[5+n+len]='\n';
    return 1;
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
        if(c[i]!=tolower(check[i]))
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