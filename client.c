/*
 * client.c
 *
 *  Created on: 2021-11-21
 *      Author: @v-barros
 */
#include "client.h"
#include "connection.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#define MAX 110
#define maxkeylength 100

char * sstr_get(replyParser*rp);

/* Turn the string into a smaller (or equal) string containing only the
 * substring specified by the 'start' and 'end' indexes.
 *
 * The interval is inclusive, so the start and end characters will be part
 * of the resulting string.
 *
 * The string is modified in-place.
 *
 * Return value:
 * -1 (error) if strlen(s) is 0 
 * -1 if start is grater than end 
 * new s len on success.
 *
 */
int strmove(char *s, ssize_t start, ssize_t end);

replyParser * new_parser(){
    replyParser * rp = (replyParser*) malloc(sizeof(replyParser));
    assert(rp);
    rp->buf = malloc(1024);
    assert(rp->buf);
    rp->len =0;
    return rp;
}
int readstdin(char *buff);

int parserFeed(replyParser * rp,const char *buff, ssize_t len){
    
    if(buff!=NULL && len >1){
        memcpy(rp->buf+rp->len,buff,len);
        rp->buf[len]=0;
        rp->len += len;
        return 1;
    }
    return 0;
}

int buffRead(context *cp){
    char buff[1024];
    ssize_t nread;
    nread = read(cp->sockfd,buff,sizeof(buff));
    if(nread<0)
        return 0;
    if(parserFeed(cp->rParser,buff,nread))
        return 1;
    return 0;
}

int buffWrite(context *cp, int * done){
    ssize_t nwrite;
    if(cp->bufflen>0){
        nwrite = write(cp->sockfd,cp->wbuff,cp->bufflen);
        if(nwrite<0){
            return 0;
        }
        if(nwrite == cp->bufflen){
            free(cp->wbuff);
            cp->bufflen=0;
        }else{
            int len = strmove(cp->wbuff,nwrite,cp->bufflen-1);
            if(len>0)
                cp->bufflen=len;
        }
    }
    if(done!=NULL) 
        *done = cp->bufflen==0?0:1;
    
    return 1;
}

int strmove(char *s, ssize_t start, ssize_t end){
    size_t newlen, len = strlen(s);
   
    if (len == 0) return -1;
    
    newlen = (start > end) ? 0 : (end-start)+1;
    if (newlen != 0) {
        if (start >= (ssize_t)len) {
            newlen = 0;
        } else if (end >= (ssize_t)len) {
            end = len-1;
            newlen = (start > end) ? 0 : (end-start)+1;
        }
    } else {
        return -1;
    }
    if (start && newlen) memmove(s, s+start, newlen);
    s[newlen] = 0;
    return newlen;
}

context * contextInit(char * serveripv4addr, u_int16_t port){
    context * cp = (struct context *) malloc(sizeof(context));
    assert(cp);
    cp->sockfd = create_conn(serveripv4addr,port);
    cp->serverport = port;
    cp->ipv4addr = serveripv4addr;
    cp->wbuff = NULL;
    cp->rParser = new_parser();
    cp->bufflen=0;
    return cp;
}

void run(int sockfd)
{
    char buff[MAX];
    char aux[MAX];
    int len;

    for (;;) {
        memset(buff,0,sizeof(buff));
        memset(aux,0,sizeof(aux));
        printf("\n>");
        len = readstdin(buff);
        len = is_valid_get(buff,len);
        if(len!=-1){
           // printf("buff \"%s\"",buff);
            len = encode_get(buff,len,aux);
           // printf("sending:\"%s\"%d\n",aux,len); // "$0$3$foo\r\n"
            write(sockfd, aux, len);
        }
        else{    
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
int readstdin(char *buff){
    int n =0;
    while ((buff[n++] = getchar()) != '\n');
    buff[n-1]=buff[n];
    int k, v;
   // printf("is_valid_set %d ", is_valid_set(&k,&v,buff,n-1));
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

char * get(context * context,char * key){
    return NULL;
}

char * set(context * context,char * key, char *value){
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

int getReply(context *cp){
    int done=0;
    do{
        if(buffWrite(cp,&done)==0)
            return 0;
    }while(!done);
    do{
        if(buffRead(cp)==0)
            return 0;        
    }while(validReply(cp->rParser)==0);
    
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


//$5$mykey\r\n
int validReply(replyParser * rp){
    if(!rp)
        return 0;
    if(rp->len<1)
        return 0;
    if(rp->buf[0]!='$')
        return 0;
    int n = stoi(rp->buf+1);
    
    if(n>0)
        if(rp->buf[digits(n)+1]!='$')
            return 0;
        else if(rp->buf[digits(n)+2+n]!='\r')
            return 0;
        else if(rp->buf[digits(n)+3+n]!='\n')
            return 0;
        else
            return 1;
    return 0;
        
}

char * sstr_get(replyParser*rp){
    if(rp)
        return rp->buf;
    return "";    
}

char * get_reply_str(context*cp){
    return sstr_get(cp->rParser);
}