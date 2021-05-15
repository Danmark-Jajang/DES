#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;
typedef unsigned int UNIT;

BYTE ep[48] = {
    32,1,2,3,4,5,4,5,
    6,7,8,9,8,9,10,11,
    12,13,12,13,14,15,16,17,
    16,17,18,19,20,21,20,21,
    22,23,24,25,24,25,26,27,
    28,29,28,29,30,31,32,1
};

BYTE s_box[8][4][16] = {
    {//1
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 
    },
    {//2
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
    },
    {//3
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
    },
    {//4
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
    },
    {//5
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
    },
    {//6
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
    },
    {//7
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
    },
    {//8
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
    }
};

BYTE p[32] = {
    16,7,20,21,29,12,28,17,
    1,15,23,26,5,18,31,10,
    2,8,24,14,32,27,3,9,
    19,13,30,6,22,11,4,25
};

UNIT Function(UNIT r, BYTE *rkey);
void EP(UNIT in, BYTE *out);
UNIT S_Box(BYTE* in);
UNIT Permutation(UNIT in);
void printText(BYTE* text);

UNIT Function(UNIT r, BYTE *rkey){
    int i;
    BYTE data[6] = {0,}; //48bit, Expansion Permutation 연산 결과 저장
    UNIT out; //return value

    EP(r, data);// 1) EP(Expansion Permutation)을 이용해 48bit로 확장

    for(i=0;i<6;i++){
        data[i] ^= rkey[i]; // 2) data XOR rkey
    }

    out = Permutation(S_Box(data)); //3) S_Box를 이용해 32비트로 압축 후 / 4) Permutation을 이용해 재배치

    return out; //1Round 끝
}

void EP(UNIT r, BYTE* out){ //32bit -> 48bit
    int i;
    UNIT mask = 0x80000000;

    for(i=0;i<48;i++){
        if(r & (mask>>ep[i]-1)){ //r의 해당 인덱스에 값이 있다면
            out[i/8] |= (BYTE)(0x80>>(i%8)); //0x80: BYTE mask
        }
    }
}

UNIT S_Box(BYTE *in){ //48bit -> 32bit
    int i, row, column, shift = 28;
    UNIT temp = 0, result = 0, mask = 0x00000080;

    //6bit가 a1, a2, a3, a4, a5, a6일 때 a1과 a6를 이용해 row를, a2, a3, a4, a5를 이용해 column을 결정한다
    for(i=0;i<48;i++){
        if(in[i/8] & (BYTE)(mask>>(i%8))){
            temp |= 0x20>>(i%6); //0x20: 0010 0000(6bit mask)
        }

        if((i+1)%6==0){
            row = ((temp&(0x20))>>4)+(temp&0x01);
            column = (temp&0x1E)>>1;
            result += ((UNIT)s_box[i/6][row][column])<<shift;

            shift -= 6;
            temp = 0;
        }
    }

    return result;
}

UNIT Permutation(UNIT in){ //32bit 배열 재배치
    int i;
    UNIT result = 0, mask = 0x80000000;

    for(i=0;i<32;i++){
        if(in & (mask>>p[i]-1)){
            result |= mask>>i;
        }
    }

    return result;
}

void printText(BYTE* text){ //half text print function(R or L)
    int i;
    printf("\n");
    for(i=0;i<4;i++){
        printf("%3x", text[i]);
    }
    printf("\n\n");
}

void main(){
    int i;
    UNIT rtext = 0xAAEEBBCC;
    UNIT out;
    BYTE data[6] = {0,};
    BYTE key[6] = {0x10, 0x29, 0x38, 0x47, 0x56, 0x11};    
    printf("\nRight text:");
    printf("\n%08x",rtext);

    out = Function(rtext, key);

    printf("\nAfter Function:");
    printf("\n%08x", out);
}