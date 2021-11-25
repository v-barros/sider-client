/*
 * client.c
 *
 *  Created on: 2021-11-21
 *      Author: @v-barros
 */
#include "client.h"
#define MAX 80

int read_command(char *buff);

void func(int sockfd)
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
            printf("sending:\"%s\"%d\n",aux,len);
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
    int n = toString(len,dest+3);
    dest[3+n]='$';
    memcpy(dest+4+n,src,len);
    dest[4+n+len]='\r';
    dest[5+n+len]='\n';
    return 1;
}

char * get(char * key){

}

char * set(char * key, char *value){

}