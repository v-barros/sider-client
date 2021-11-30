/*
 * utils.c
 *
 *  Created on: 2021-11-30
 *      Author: @v-barros
 */
#include "utils.h"

int itostring(int n,char *dest){
    if(n<0)
        return -1;
        
    int len=0;
    int aux=0;
    char temp[10];

    while(n){
        temp[len++] =n%10;
        n=n/10;
    }
    aux = len;
    while(aux){
        dest[len-aux]=itochar(temp[aux-1]);
        aux--;
    }
    return len;
}

char itochar(int n){
    if(n>=0&&n<=9)
        return n+0x30;
    return 0x20;
}