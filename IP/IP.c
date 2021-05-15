#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;
typedef unsigned int UNIT;

BYTE ip[64] = {
    58,50,42,34,26,18,10,2,
    60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,
    64,56,48,40,32,24,16,8,
    57,49,41,33,25,17, 9,1,
    59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,
    63,55,47,39,31,23,15,7
};

BYTE Inv_ip[64] = {
    40, 8,48,16,56,24,64,32,
    39, 7,47,15,55,23,63,31,
    38, 6,46,14,54,22,62,30,
    37, 5,45,13,53,21,61,29,
    36, 4,44,12,52,20,60,28,
    35, 3,43,11,51,19,59,27,
    34, 2,42,10,50,18,58,26,
    33, 1,41, 9,49,17,57,25
};

void IP(BYTE *in, BYTE *out);
void Inv_IP(BYTE *in, BYTE *out);
void printText(BYTE *in);

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

void printText(BYTE *text){
    int i;
    printf("\n");
    for(i=0;i<8;i++){
        printf("%3x",text[i]);
    }
    printf("\n\n");
}

void main(){
    BYTE text[8] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF};
    BYTE c_text[8] = {0,};
    BYTE d_text[8] = {0,};

    printf("plain text:");
    printText(text);

    IP(text, c_text);
    printf("After Initial Permutation:");
    printText(c_text);

    Inv_IP(c_text, d_text);
    printf("After Inverse Initial Permutation:");
    printText(d_text);
}