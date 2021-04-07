#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DES.h"

void main(){
    int i;
    int msg_len = 0, block_count = 0;
    BYTE p_text[128] = {0,};
    BYTE c_text[128] = {0,};
    BYTE key[9] = {0,};

    printf("Enter PlainText\t>>");
    gets(p_text);

    printf("Enter Key\t>>");
    scanf("%s",key);
    msg_len = (int)strlen((char *)p_text);
    block_count = (msg_len%BLOCK_SIZE) ? (msg_len/BLOCK_SIZE+1) : (msg_len/BLOCK_SIZE);

    for(i=0;i<block_count;i++){
        DES_Encryption(&p_text[i*BLOCK_SIZE], &c_text[i*BLOCK_SIZE],key);
    }

    printf("Cipher\t>>");
    for(i=0;i<block_count*BLOCK_SIZE;i++){
        printf("%x",c_text[i]);
    }
    printf("\nd");
}

void DES_Encryption(BYTE *p_text, BYTE *result, BYTE *key){
    int i;
    BYTE data[BLOCK_SIZE] = {0,};
    BYTE round_key[16][6] = {0,}; //16Round, 48bits
    UNIT L=0, R=0;

    Key_generater(key, round_key);
    IP(p_text, data);

    BtoW(data, &L, &R);

    for(i=0;i<DES_ROUND;i++){
        L = L ^ f(R, round_key[i]);

        if(i!=DES_ROUND-1){
            swap(&L, &R);
        }
    }

    WtoB(L,R,data);
    IP_1(data,result);
}