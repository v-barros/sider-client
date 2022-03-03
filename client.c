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
#include <limits.h>
#define MAX 110
#define maxkeylength 100

char * sstr_get(replyParser*rp);

//void freeReply(context *scontext);

int getReply(context *cp);

int readstdin(char *buff);

void parse(char ** dest, replyParser  *rp);

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
    rp->buf = (char *)malloc(sizeof(char)*1024);
    assert(rp->buf);
    rp->len =0;
    return rp;
}

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
        *done = cp->bufflen==0?1:0;
    
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
    cp->wbuff = (char *) malloc(sizeof(char)*1024);
    assert(cp->wbuff);
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
         //   len = encode_get(buff,len,aux);
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


//$1$3$3$foobar
int encode_set(context * cp,char *key,char *value,int keylen,int valuelen){
    int chars;
    cp->wbuff[0]='$';
    cp->wbuff[1]='1';
    cp->wbuff[2]='$';
    chars = itostring(keylen,cp->wbuff+3);
    cp->wbuff[3+chars]='$';
    chars+= itostring(valuelen,cp->wbuff+4+chars);
    cp->wbuff[4+chars]='$';
    memcpy(cp->wbuff+5+chars,key,keylen);
    memcpy(cp->wbuff+5+chars+keylen,value,valuelen);
    cp->wbuff[5+chars+keylen+valuelen]='\r';    
    cp->wbuff[6+chars+keylen+valuelen]='\n';    
    cp->wbuff[7+chars+keylen+valuelen]=0;    
    cp->bufflen=6+chars+keylen+valuelen;
    return 6+chars+keylen+valuelen;
}

int encode_get(context * cp, char *key,int keylen){
    cp->wbuff[0]='$';
    cp->wbuff[1]='0';
    cp->wbuff[2]='$';
    int n = itostring(keylen,cp->wbuff+3);
    cp->wbuff[3+n]='$';
    memcpy(cp->wbuff+4+n,key,keylen);
    cp->wbuff[4+n+keylen]='\r';    
    cp->wbuff[5+n+keylen]='\n';
    cp->wbuff[6+n+keylen]=0;
    cp->bufflen=6+n+keylen;
    return 6 + n +keylen;
}

void freeReply(context *scontext){
    *(scontext->rParser->buf)='0';
    scontext->rParser->len=0;
}

char * get(context * scontext,char * key){
    int klen;
    if(!key)
        return NULL;
    klen = strlen(key);

    encode_get(scontext,key,klen);
    if(getReply(scontext)){
        char *c= get_reply_str(scontext);
        freeReply(scontext);
        return c;
    }
    else {
        return NULL;
    }

}

char * set(context * scontext,char * key, char *value){
    int klen, vlen;
    if(!key || !value)
        return NULL;

    klen = strlen(key);
    vlen = strlen(value);

    encode_set(scontext,key,value,klen,vlen);
    if(getReply(scontext)){
        char *c= get_reply_str(scontext);
        freeReply(scontext);
        return c;
    }
    else {
        return NULL;
    }
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
    return 1;
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
    char * c = rp->buf;

    if(rp->len<1)
        return 0;
    if(rp->buf[0]!='$')
        return 0;
    
    int n = stoi(rp->buf+1);
    if(n!=INT_MAX)
        if(n==-1)
            return *(rp->buf+5)=='\n';
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

char * get_reply_str(context*cp){
    char * c;
    parse(&c,cp->rParser);
    return c;
}

void parse(char ** dest, replyParser * rp ){
    if(rp->len<=1)
    {
        *dest = malloc(5);
        memcpy(*dest,"null",5);
    }
    int len = stoi((rp->buf+1));

    if(len == -1){
        *dest = malloc(5);
        memcpy(*dest,"null",5);
    }else{
        *dest = malloc(len+1);
        memcpy(*dest,rp->buf+(digits(len)+2),len);
        *(*dest+len)='\0';
    }
}