/*
 * utils.c
 *
 *  Created on: 2021-11-30
 *      Author: @v-barros
 */
#include "utils.h"
#include <limits.h>
#include <ctype.h>
#include <math.h>

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

int stoi(char * str){
    __uint32_t n=0; 
    
    if(*str=='-')
    {
        str++;
        while (*str!='$' && n<INT_MAX)
        {
            n = (n*10)+(*str - '0');
            str++;
        }
        if (n<INT_MAX)
            return n*-1;
    }else
    {
        while (*str!='$' && n<INT_MAX)
        {
            n = (n*10)+(*str - '0');
            str++;
        }
        if (n<INT_MAX)
            return n;
    }
    return INT_MAX;
}

__int32_t digits(int v){
    v=abs(v);
    return  1
    + (__int32_t)(v>=10)
    + (__int32_t)(v>=100)
    + (__int32_t)(v>=1000)
    + (__int32_t)(v>=10000)
    + (__int32_t)(v>=100000)
    + (__int32_t)(v>=1000000)
    + (__int32_t)(v>=10000000)
    + (__int32_t)(v>=100000000)
    + (__int32_t)(v>=1000000000);
}