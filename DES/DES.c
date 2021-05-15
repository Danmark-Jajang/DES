#include <stdio.h>
#include <stdlib.h>

#define DES_ROUND 16

typedef unsigned char BYTE;
typedef unsigned int UNIT;

BYTE ip[64] = { //Initial Permutation Table
    58,50,42,34,26,18,10,2,
    60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,
    64,56,48,40,32,24,16,8,
    57,49,41,33,25,17, 9,1,
    59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,
    63,55,47,39,31,23,15,7
};

BYTE Inv_ip[64] = { //Inverse Initial Permutation Table
    40, 8,48,16,56,24,64,32,
    39, 7,47,15,55,23,63,31,
    38, 6,46,14,54,22,62,30,
    37, 5,45,13,53,21,61,29,
    36, 4,44,12,52,20,60,28,
    35, 3,43,11,51,19,59,27,
    34, 2,42,10,50,18,58,26,
    33, 1,41, 9,49,17,57,25
};

BYTE pc_1[56] = { //Parity Drop Table
    57,49,41,33,25,17,9,
    1,58,50,42,34,26,18,
    10,2,59,51,43,35,27,
    19,11,3,60,52,44,36,
    63,55,47,39,31,23,15,
    7,62,54,46,38,30,22,
    14,6,61,53,45,37,29,
    21,13,5,28,20,12,4
};

BYTE pc_2[48] = { //Compression D-Box
    14,17,11,24,1,5,
    3,28,15,6,21,10,
    23,19,12,4,26,8,
    16,7,27,20,13,2,
    41,52,31,37,47,55,
    30,40,51,45,33,48,
    44,49,39,56,34,53,
    46,42,50,36,29,32
};

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

void IP(BYTE *in, BYTE *out);
void Inv_IP(BYTE *in, BYTE *out);
void PC1(BYTE *in, BYTE *out);
void makeBit28(UNIT *left, UNIT *right, BYTE* pkey);
UNIT leftShift(UNIT p, int r);
void PC2(UNIT left, UNIT right, BYTE *out);
void keyGenerator(BYTE *key, BYTE roundKey[16][6]);
UNIT function(UNIT r, BYTE *rkey);
void EP(UNIT in, BYTE *out);
UNIT S_Box(BYTE* in);
UNIT Permutation(UNIT in);
void BtoW(BYTE *in, UNIT *x, UNIT *y);
void WtoB(UNIT left, UNIT right, BYTE *out);
void DES_Encryption(BYTE *ptext, BYTE* result, BYTE *key);
void DES_Decryption(BYTE *ctext, BYTE* resutl, BYTE *key);
void printText(BYTE *text);

//IP
void IP(BYTE *in, BYTE *out){
    int i;
    BYTE index, bit, mask = 0x80;

    for(i=0;i<64;i++){
        index = (ip[i]-1)/8;
        bit = (ip[i]-1)%8;

        if(in[index] & (mask>>bit)){
            out[i/8] |= mask>>(i%8);
        }
    }
}

void Inv_IP(BYTE *in, BYTE *out){
    int i;
    BYTE index, bit, mask = 0x80;

    for(i=0;i<64;i++){
        index = (Inv_ip[i]-1)/8;
        bit = (Inv_ip[i]-1)%8;

        if(in[index] & (mask>>bit)){
            out[i/8] |= mask>>(i%8);
        }
    }
}

//RoundFunction
UNIT function(UNIT r, BYTE *rkey){
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

//KeyGenerator
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

void makeBit28(UNIT *left, UNIT *right, BYTE* pkey){ //left, right: 28bit pointer, pkey: array[7], divide in 2 part
    int i;
    BYTE mask = 0x80;

    for(i=0;i<56;i++){
        if(i<28){ //0~27, left
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

UNIT leftShift(UNIT n, int r){ //p: left or right, r: Round
    int r_shift[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

    if(r_shift[r]==1){
        n = ((n<<1)+(n>>27)) & 0x0FFFFFFF; //0x0FFFFFFF: 최상단의 4bit제거를 위해 AND 연산
    }
    else{
        n = ((n<<2)+(n>>26)) & 0x0FFFFFFF; //0x0FFFFFFF: 최상단의 4bit제거를 위해 AND 연산
    }

    return n;
}

void PC2(UNIT left, UNIT right, BYTE *out){ //left, right의 56bit를 48bit로 압축 및 재배치한 결과를 최종 서브키 결정
    int i;
    UNIT mask = 0x08000000; //48bit mask

    for(i=0;i<48;i++){
        if((pc_2[i]-1)<28){ //left의 범위인지 확인, 0~27
            if(left & (mask>>(pc_2[i]-1))){
                out[i/8] |= 0x80>>(i%8);
            }
        }
        else{ //right의 범위, 28~47
            if(right & (mask>>(pc_2[i]-1-28))){
                out[i/8] |= 0x80>>(i%8);
            }
        }
    }
}

void keyGenerator(BYTE *key, BYTE roundKey[16][6]){
    int i;
    BYTE pc1_res[7] = {0,};
    UNIT left = 0, right = 0;

    PC1(key, pc1_res);
    makeBit28(&left, &right, pc1_res);

    //16라운드에서 사용될 subKey추출
    for(i=0;i<16;i++){
        left = leftShift(left, i);
        right = leftShift(right, i);

        PC2(left, right, roundKey[i]);
    }
}

//Util
void BtoW(BYTE *in, UNIT *x, UNIT *y){ //64bit -> 32bit * 2
    int i;

    for(i=0;i<8;i++){
        if(i<4){
            *x |= (UNIT)in[i]<<(24-(i*8)); //1BYTE만큼 shift하여 왼쪽부터 채워넣기
        }
        else{
            *y |= (UNIT)in[i]<<(56-(i*8)); //i는 4, 5, 6, 7의 값을 가지므로 0부터 시작하는 효과를 내기 위해 56에서 감소(8*4 = 32)
        }
    }
}

void WtoB(UNIT left, UNIT right, BYTE *out){ //32bit * 2 -> 64bit
    int i;
    UNIT mask = 0xFF000000;

    for(i=0;i<8;i++){
        if(i<4){
            out[i] = (left & (mask>>i*8))>>(24-(i*8));
        }
        else{
            out[i] = (right & (mask>>(i-4)*8))>>(56-(i*8));
        }
    }
}

void swap(UNIT *left, UNIT *right){
    UNIT temp = 0;
    temp = *left;
    *left = *right;
    *right = temp;
}

void printText(BYTE *text){
    int i;
    printf("\n");
    for(i=0;i<8;i++){
        printf("%02X ",text[i]);
    }
}

//Encryption
void DES_Encryption(BYTE *ptext, BYTE* result, BYTE *key){
    int i;
    BYTE expansion[8] = {0,}; 
    BYTE roundKey[16][6] = {0,};
    UNIT left = 0, right = 0;

    keyGenerator(key, roundKey);

    IP(ptext, expansion);

    BtoW(expansion, &left, &right);

    for(i=0;i<16;i++){
        left = left ^ function(right, roundKey[i]);
        if(i!=DES_ROUND-1){
            swap(&left, &right);
        }
    }

    WtoB(left, right, expansion);
    Inv_IP(expansion, result);
}

//Decryption
void DES_Decryption(BYTE *ctext, BYTE* result, BYTE *key){
    int i;
    BYTE expansion[8] = {0,};
    BYTE roundKey[16][6] = {0,};
    UNIT left = 0, right = 0;

    keyGenerator(key, roundKey);

    IP(ctext, expansion);

    BtoW(expansion, &left, &right);

    for(i=0;i<16;i++){
        left = left ^ function(right, roundKey[DES_ROUND-i-1]);
        if(i!=DES_ROUND-1){
            swap(&left, &right);
        }
    }

    WtoB(left, right, expansion);
    Inv_IP(expansion, result);
}

//Main
void main(){
    BYTE pText[8] = {0x12, 0x34, 0x56, 0xAB, 0xCD, 0x13, 0x25, 0x36};
    BYTE cText[8] = {0,};
    BYTE dText[8] = {0,};
    BYTE key[8] = {0xAA, 0xBB, 0x09, 0x18, 0x27, 0x36, 0xCC, 0xDD};
    printf("\nPlain Text:");
    printText(pText);
    printf("\nKey Text:");
    printText(key);

    DES_Encryption(pText, cText, key);
    printf("\nAfter DES_Encryption:");
    printText(cText);

    DES_Decryption(cText, dText, key);
    printf("\nAfter DES_Decryption:");
    printText(dText);
}