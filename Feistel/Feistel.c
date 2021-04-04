#define _CRT_NO_SECURE_WARNINGS_
#include <stdio.h>
#define BLOCK_SIZE 6
#define ROUND_NUM 2

char F1(char); //key1
char F2(char); //key2
char Feistel_Encryption(char);
char Feistel_Decryption(char);
char f_cal(int,char,char);

void main(){
    char plain_t = 0x2B; //101011(2)
    char cipher_t, decipher_t;
    int i=0, t=0;

    //Print Plain Text
    printf("Enter Plain Text(Char type)>>");
    scanf("%c",&plain_t);
    printf("Plain Text\t>>");
    for(i=BLOCK_SIZE-1;i>=0;i--){
        t = (plain_t>>i)&0x01; //&0x01 -> to make binary(0 or 1)
        printf("%d ",t);
    }  
    printf("\n");

    cipher_t = Feistel_Encryption(plain_t);

    //Print Cipher Text
    printf("Cipher Text\t>>");
    for(i=BLOCK_SIZE-1;i>=0;i--){
        t = (cipher_t>>i)&0x01;
        printf("%d ",t);
    }  
    printf("\n");

    decipher_t = Feistel_Decryption(cipher_t);

    //Print Decipher Text
    printf("Decipher Text\t>>");
    for(i=BLOCK_SIZE-1;i>=0;i--){
        t = (decipher_t>>i)&0x01;
        printf("%d ",t);
    }  
    printf("\n");
}

char F1(char in){
         if(in==0x00) return 0x05; //0->5
    else if(in==0x01) return 0x02; //1->2
    else if(in==0x02) return 0x03; //2->3
    else if(in==0x03) return 0x06; //3->6
    else if(in==0x04) return 0x04; //4->4
    else if(in==0x05) return 0x01; //5->1
    else if(in==0x06) return 0x07; //6->7
    else if(in==0x07) return 0x00; //7->0
}

char F2(char in){
         if(in==0x00) return 0x04; //0->4
    else if(in==0x01) return 0x00; //1->0
    else if(in==0x02) return 0x03; //2->3
    else if(in==0x03) return 0x07; //3->7
    else if(in==0x04) return 0x06; //4->6
    else if(in==0x05) return 0x05; //5->5
    else if(in==0x06) return 0x01; //6->1
    else if(in==0x07) return 0x02; //7->2
}

char f_cal(int i,char left,char right){
    if(i==0) return left^F1(right); //Round 1
    if(i==1) return left^F2(right); //Round 2
}

char Feistel_Encryption(char in){
    int i=0;
    char temp, left, right;

    left = (in>>3)&0x07; //0x07 => 111, make it three-digit binary
    right = in&0x07;

    for(i=0;i<ROUND_NUM;i++){
        left = f_cal(i,left,right);
        if(i!=ROUND_NUM-1){ //switching, do not switching at the end
            temp = left;
            left = right;
            right = temp;
        }
    }

    return (left<<3)|right; //left+right, make six-digit binary
}

char Feistel_Decryption(char in){
    int i=0;
    char temp, left, right;

    left = (in>>3)&0x07;
    right = in&0x07;

    for(i=ROUND_NUM-1;i>=0;i--){
        left = f_cal(i,left,right);

        if(i!=0){
            temp = left;
            left = right;
            right = temp;
        }
    }

    return (left<<3)|right;
}