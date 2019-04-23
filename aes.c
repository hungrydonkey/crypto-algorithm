#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define Nk 4
#define Nr 10
#define Nb 4

static const uint8_t Sbox[16][16] =
{
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

static uint8_t Plaintext[4*Nb] =
{
    0x32,0x43,0xf6,0xa8,
    0x88,0x5a,0x30,0x8d,
    0x31,0x31,0x98,0xa2,
    0xe0,0x37,0x07,0x34
};

static const uint8_t key[4*Nk] =
{
    0x2b,0x7e,0x15,0x16,
    0x28,0xae,0xd2,0xa6,
    0xab,0xf7,0x15,0x88,
    0x09,0xcf,0x4f,0x3c
};

static const uint32_t Rcon[] =
{
  0x01000000, 0x02000000, 0x04000000, 0x08000000,
  0x10000000, 0x20000000, 0x40000000, 0x80000000,
  0x1B000000, 0x36000000
};

void KeyExpansion(uint32_t w[]){
    uint32_t temp;
    int i = 0;
    while(i < Nk){
        w[i] = (key[4*i] <<24) ^ (key[4*i+1] <<16) ^ (key[4*i+2] <<8) ^ key[4*i+3];
        i++;
    }
    i = Nk;
    while(i < Nb*(Nr+1)){
        temp = w[i-1];
        if(i%Nk == 0){
            temp = (temp >>24) ^ (temp <<8);
            printf("after rotword:%#x %#x\n",temp,Sbox[temp >>28][((temp >>24) & 0x0f)]);
            temp = (Sbox[temp >>28][(temp >>24) & 0x0f] <<24) ^
                   (Sbox[(temp >>20)&0x00f][(temp >>16) & 0x000f] <<16) ^
                   (Sbox[(temp >>12)&0x0000f][(temp >>8) & 0x00000f] <<8) ^
                   (Sbox[(temp >>4)&0x000000f][temp& 0x0000000f]);
            printf("after subword:%#x\n",temp);
            temp = temp ^ Rcon[i/Nk-1];
            printf("after ^rcon:%#x\n",temp);
        }
        w[i] = w[i-Nk] ^ temp;
        printf("%d : %#x\n",i,w[i]);
        i++;
    }
}

void AddRoundKey(uint8_t state[],uint32_t w[],int n){
        for(int i = 0;i < Nb;i++){
            state[4*i]   = state[4*i] ^ (w[n+i] >>24);
            //printf("%#x",state[4*i]);
            state[4*i+1] = state[4*i+1] ^ (uint8_t)(w[n+i] >>16);
            //printf(" %#x",state[4*i+1]);
            state[4*i+2] = state[4*i+2] ^ (uint8_t)(w[n+i] >>8);
            //printf(" %#x",state[4*i+2]);
            state[4*i+3] = state[4*i+3] ^ (uint8_t)w[n+i];
            //printf(" %#x\n",state[4*i+3]);
        }
}

void SubBytes(uint8_t state[]){
    for(int i = 0;i < Nb*4;i++)
        state[i] = Sbox[state[i]>>4][state[i]&0x0f];
}

void ShiftRows(uint8_t state[]){
    uint8_t temp,temp1;
    int i;
    temp = state[1];
    for(i = 0;i < Nb-1;i++){
        state[1+i*4] = state[1+(i+1)*4];
    }
    state[1+(i+1)*3] = temp;
    temp  = state[2];
    temp1 = state[2+Nb];
    state[2]    = state[2+Nb*2];
    state[2+Nb] = state[2+Nb*3];
    state[2+Nb*2] = temp;
    state[2+Nb*3] = temp1;
    temp = state[3+Nb*3];
    for(i = Nb-1;i > 0;i--){
        state[3+i*4] = state[3+(i-1)*4];
    }
    state[3] = temp;
}

uint8_t mul(uint8_t a,uint8_t b){
    uint8_t p = 0;
	while (a && b) {
        if (b & 1)
            p ^= a;

        if (a & 0x80)
            a = (a << 1) ^ 0x11b;
        else
            a <<= 1;
        b >>= 1;
	}
	return p;
}



void MixColumns(uint8_t state[]){
    uint8_t temp[Nb] = {0};
    for(int i = 0;i < Nb;i++){
        for(int j = 0;j < Nb;j++){
            temp[j] = state[4*i+j];
        }
        state[4*i]   = mul(0x2,temp[0]) ^ mul(0x3,temp[1]) ^ temp[2] ^ temp[3];
        state[4*i+1] = mul(0x2,temp[1]) ^ mul(0x3,temp[2]) ^ temp[0] ^ temp[3];
        state[4*i+2] = mul(0x2,temp[2]) ^ mul(0x3,temp[3]) ^ temp[0] ^ temp[1];
        state[4*i+3] = mul(0x2,temp[3]) ^ mul(0x3,temp[0]) ^ temp[1] ^ temp[2];
    }
}


/*
static inline uint8_t mul(uint8_t a) {
    return (a&0x80) ? ((a<<1)^0x1b) : (a<<1);      //a*0x02(G2^8)
}

void MixColumns(uint8_t state[]){
    uint8_t temp[Nb] = {0};
    uint8_t t;
    for(int i = 0;i < Nb;i++){
        memcpy(temp,&(state[4*i]),sizeof(uint8_t)*Nb);
        t = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
        state[4*i]   = mul(temp[0] ^ temp[1]) ^ temp[0] ^ t;
        state[4*i+1] = mul(temp[1] ^ temp[2]) ^ temp[1] ^ t;
        state[4*i+2] = mul(temp[2] ^ temp[3]) ^ temp[2] ^ t;
        state[4*i+3] = mul(temp[3] ^ temp[0]) ^ temp[3] ^ t;
    }
}
*/
int main(void){

    uint8_t Cipher[4*Nb]  = {0};
    uint8_t state[4*Nb]   = {0};
    uint32_t w[Nb*(Nr+1)] = {0};      //Round Key

    memcpy(state,Plaintext,sizeof(uint8_t)*4*Nb);

    KeyExpansion(w);
    AddRoundKey(state,w,0);
    /*
    SubBytes(state);
    for(int i = 0;i < Nb;i++)
        printf("%x %x %x %x\n",state[4*i],state[4*i+1],state[4*i+2],state[4*i+3]);
    ShiftRows(state);
    for(int i = 0;i < Nb;i++)
        printf("%x %x %x %x\n",state[4*i],state[4*i+1],state[4*i+2],state[4*i+3]);
    MixColumns(state);
    for(int i = 0;i < Nb;i++)
        printf("%x %x %x %x\n",state[4*i],state[4*i+1],state[4*i+2],state[4*i+3]);
    */
    for(int round = 1;round <= Nr-1;round++){
        SubBytes(state);
        for(int i = 0;i < Nb;i++)
            printf("%x %x %x %x\n",state[4*i],state[4*i+1],state[4*i+2],state[4*i+3]);
        ShiftRows(state);
        for(int i = 0;i < Nb;i++)
            printf("%x %x %x %x\n",state[4*i],state[4*i+1],state[4*i+2],state[4*i+3]);
        MixColumns(state);
        for(int i = 0;i < Nb;i++)
            printf("%x %x %x %x\n",state[4*i],state[4*i+1],state[4*i+2],state[4*i+3]);
        AddRoundKey(state,w,round*Nb);
        for(int i = 0;i < Nb;i++)
            printf("%x %x %x %x\n",state[4*i],state[4*i+1],state[4*i+2],state[4*i+3]);
        printf("\n");
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state,w,Nr*Nb);

    memcpy(Cipher,state,sizeof(uint8_t)*4*Nb);
    for(int i = 0;i < Nb;i++)
        printf("%x %x %x %x\n",Cipher[4*i],Cipher[4*i+1],Cipher[4*i+2],Cipher[4*i+3]);

    return 0;
}
