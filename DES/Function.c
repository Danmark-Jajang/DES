#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DES.h"

extern ip[64];
extern ip_1[64];
extern E[48];
extern S_BOX[8][4][16];
extern P[32];
extern PC_1[56];
extern PC_2[48];

void IP(BYTE *in, BYTE *out){
    int i;
    BYTE index, bit, mask = 0x80;

    for(i=0;i<64;i++){
        index = (ip[i]-1)/8;
        bit = (ip[i]-1)%8;

        if(in[index]&(mask>>bit)){
            out[i/8] = out[i/8]|(mask>>(i%8));
        }
    }
}

void IP_1(BYTE *in, BYTE *out){
    int i;
    BYTE index, bit, mask = 0x80;

    for(i=0;i<64;i++){
        index = (ip_1[i]-1)/8;
        bit = (ip_1[i]-1)%8;

        if(in[index]&(mask>>bit)){
            out[i/8] = out[i/8]|(mask>>(i%8));
        }
    }
}

void EP(UNIT r, BYTE *out){ //32bits -> 48bits
    int i;
    UNIT mask = 0x800000;

    for(i=0;i<48;i++){
        if(r&(mask>>E[i]-1)){
            out[i/8] = out[i/8] | (BYTE)(0x80>>(i%8));
        }
    }
}

UNIT S_box(BYTE *in){ //48bits -> 32bits
    int i, row, column, shift = 28;
    UNIT temp = 0, result = 0, mask = 0x00000080;

    for(i=0;i<48;i++){
        if(in[i/8]&(BYTE)(mask>>(i%8))){
            temp = 0x20 | mask>>(i%6); //0x20: 10 0000(2)
        }

        if((i+1)%6==0){
            row = ((temp&0x20)>>4)+(temp&0x01); //a1a6
            column = (temp&0x1E); //a2a3a4a5, 0x1E: 01 1110(2)
            result = result + (UNIT)(S_BOX[i/6][row][column]<<shift);
            shift -= 4;
            temp = 0;
        }
    }
    return result;
}

UNIT Permutation(UNIT in){
    int i;
    UNIT out = 0, mask = 0x80000000;

    for(i=0;i<32;i++){
        if(mask>>(P[i]-1)){ 
            out = out | (mask>>i);
        }
    }
}

UNIT f(UNIT r, BYTE *r_key){ //Ri, key_round
    int i;
    BYTE data[6] = {0,};
    UNIT out = 0;

    EP(r,data);

    for(i=0;i<6;i++){
        data[i] = data[i] ^ r_key[i]; //Ri xor Key
    }
    out  = Permutation(S_box(data));

    return out;
}

void PC1(BYTE *in, BYTE *out){ //64bits -> 56bits
    int i, index, bit;
    UNIT mask = 0x00000080;

    for(i=0;i<56;i++){
        index = (PC_1[i]-1)/8;
        bit = (PC_1[i]-1)%8;

        if(in[index]&(BYTE)(mask>>bit)){
            out[i/8] = out[i/8] | (BYTE)(mask>>(i%8));
        }
    }
}

void Make_28bit(UNIT *l, UNIT *r, BYTE *data){
    int i;
    BYTE mask = 0x80;

    for(i=0;i<56;i++){
        if(i<28){
            if(data[i/8]&(mask>>i%8)){
                *l = *l | (0x08000000>>i);
            }
        }
        else{
            if(data[i/8]&(mask>>i%8)){
                *r = *r | (0x08000000>>(i-28));
            }
        }
    }
}

UNIT Cyc_shift(UNIT n, int r){
    int shift[16] = {1,1,2,2,2,2,2,2,2,1,2,2,2,2,2,1};

    if(shift[r]==1){
        n = ((n<<1)+(n>>27))&0x08FFFFFF;
    }
    else{
        n = ((n<<2)+(n>>26))&0x08FFFFFF;
    }

    return n;
}

void PC2(UNIT l, UNIT r, BYTE *out){ //28bits+28bits -> 48bits key
    int i;
    UNIT mask = 0x08000000;

    for(i=0;i<48;i++){
        if(PC_2[i]<28){ //l
            if(l&(mask>>(PC_2[i]-1))){
                out[i/8] = out[i/8] | (0x80>>(i%8));
            }
        }
        else{
            if(r&(mask>>(PC_2[i]-1-28))){
                out[i/8] = out[i/8] | (0x80>>(i%8));
            }
        }
    }
}

void BtoW(BYTE *in, UNIT *l, UNIT *r){
    int i;

    for(i=0;i<8;i++){
        if(i<4){
            *l = *l | (UNIT)in[i]<<24-(i*8);
        }
        else{
            *r = *r | (UNIT)in[i]<<(56-(i%8));
        }
    }
}

void WtoB(BYTE l, UNIT r, BYTE *out){
    int i;
    UNIT mask = 0xFF000000;

    for(i=0;i<8;i++){
        if(i<4){
            out[i] = (l&(mask>>i%8))>>(24-(i*8));
        }
        else{
            out[i] = (r&(mask>>(i-4)*8))>>(56-(i*8));
        }
    }
}

void swap(UNIT *L, UNIT *R){
    UNIT temp;
    temp = *L;
    *L = *R;
    *R = temp;
}