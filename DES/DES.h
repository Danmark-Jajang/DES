#ifndef _DES_H_
#define _DES_H_

//Define Constant
#define BLOCK_SIZE 6
#define DES_ROUND 16

//Type Define
typedef unsigned int UNIT;
typedef unsigned char BYTE;

//Define Function
void IP(BYTE *in, BYTE *out);
void IP_1(BYTE *in, BYTE *out);
UNIT f(UNIT r, BYTE *r_key);
void EP(UNIT r, BYTE *out);
UNIT S_box(BYTE *in);
UNIT Permutation(UNIT in);

void Key_generater(BYTE key, BYTE *key_round);
void PC1(BYTE *in, BYTE *out);
void Make_28bit(UNIT *l, UNIT *r, BYTE *data);
UNIT Cyc_shift(UNIT n, int r);
void PC2(UNIT l, UNIT r, BYTE *out);
void DES_Encryption(BYTE *p_text, BYTE *result, BYTE *key);
//void DES_Decryption(BYTE *c_text, BYTE *result, BYTE *key);

void BtoW(BYTE *data, UNIT *l, UNIT *r);
void WtoB(BYTE l, UNIT r, BYTE *out);
void swap(UNIT *L, UNIT *R);
#endif