#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;
typedef unsigned int UNIT;

BYTE pc_1[56] = {
    57,49,41,33,25,17,9,
    1,58,50,42,34,26,18,
    10,2,59,51,43,35,27,
    19,11,3,60,52,44,36,
    63,55,47,39,31,23,15,
    7,62,54,46,38,30,22,
    14,6,61,53,45,37,29,
    21,13,5,28,20,12,4
};

BYTE pc_2[48] = {
    14,17,11,24,1,5,
    3,28,15,6,21,10,
    23,19,12,4,26,8,
    16,7,27,20,13,2,
    41,52,31,37,47,55,
    30,40,51,45,33,48,
    44,49,39,56,34,53,
    46,42,50,36,29,32
};

void PC1(BYTE *in, BYTE *out);
void makeBit28(UNIT *left, UNIT *right, BYTE* pkey);
UNIT leftShift(UNIT p, int r);
void PC2(UNIT left, UNIT right, BYTE *out);

void PC1(BYTE *in, BYTE *out){ //in: 64bit, out: 56bit
    int i, index, bit;
    BYTE mask = 0x80;

    for(i=0;i<56;i++){
        index = (pc_1[i]-1)/8;
        bit = (pc_1[i]-1)%8;

        if(in[index] & (mask>>bit)){
            out[i/8] |= (mask>>(i%8));
        }
    }
}

void makeBit28(UNIT *left, UNIT *right, BYTE* pkey){ //left, right: 28bit pointer, pkey: array[7], divide 2 part
    int i;
    BYTE mask = 0x80;

    for(i=0;i<56;i++){
        if(i<28){
            if(pkey[i/8] & (mask>>(i%8))){
                *left |= 0x08000000>>i; //0x08000000: 28bit mask
            }
        }
        else{
            if(pkey[i/8] & mask>>(i%8)){
                *right |= 0x08000000>>(i-28);
            }
        }
    }
}

UNIT leftShift(UNIT p, int r){ //p: left or right, r: Round
    int r_shift[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
    UNIT out = 0;

    if(r_shift[r]==1){
        out = ((p<<1)+(p>>27)) & 0x0FFFFFFF; //0x0FFFFFFF: 최상단의 4bit제거를 위해 AND 연산
    }
    else{
        out = ((p<<2)+(p>>26)) & 0x0FFFFFFF; //0x0FFFFFFF: 최상단의 4bit제거를 위해 AND 연산
    }

    return out;
}

void PC2(UNIT left, UNIT right, BYTE *out){ //left, right의 56bit를 48비트로 압축 및 재배치한 결과를 최종 라운드키로 결정
    int i;
    UNIT mask = 0x08000000; //48bit mask

    for(i=0;i<48;i++){
        if((pc_2[i]-1)<28){ //left의 범위인지 확인
            if(left & (mask>>(pc_2[i]-1))){
                out[i/8] |= (0x80>>(i%8));
            }
        }
        else{ //right의 범위
            if(right & (mask>>(pc_2[i]-1))){
                out[i/8] |= (0x80>>(i%8));
            }
        }
    }
}

void main(){
    int i;
    BYTE key[8] = {0xAE, 0x8C, 0x7C, 0x91, 0x76, 0x66, 0x77, 0x88}; //randomly enter
    BYTE pkey[7] = {0,};
    BYTE nkey[6] = {0,};
    UNIT left = 0, right = 0;

    printf("\nkey:\n");
    for(i=0;i<8;i++){
        printf("%3x",key[i]);
    }

    PC1(key, pkey);
    printf("\nAfter PC1:\n");
    for(i=0;i<7;i++){
        printf("%3x",pkey[i]);
    }

    makeBit28(&left, &right, pkey);
    printf("\nLeft: ");
    printf("%3x",left);
    printf("\nRight:");
    printf("%3x",right);

    left = leftShift(left, 1);
    right = leftShift(right, 1);

    printf("\n\nLeft: ");
    printf("%3x",left);
    printf("\nRight:");
    printf("%3x",right);

    PC2(left, right, nkey);
    printf("\nnew subKey:");
    for(i=0;i<6;i++){
        printf("%3x", nkey[i]);
    }
}