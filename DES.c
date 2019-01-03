#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define Nk 64    //length of key
#define Nr 16   //round
int Kn[16][48] ={0};  //round key

struct Kshel{
    int c[28];
    int d[28];
};

struct round{
    int l[32];
    int r[32];
};

static int key[Nk] =
{
    1,0,1,0,1,1,1,0,
    1,1,1,1,1,1,0,1,
    1,0,1,0,1,1,0,1,
    1,0,1,1,0,1,1,0,
    0,0,1,0,0,0,1,1,
    0,0,1,0,1,1,1,1,
    1,1,0,1,0,0,1,1,
    1,0,1,0,0,1,1,1,
};

static int PC1c[28] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36
};

static int PC1d[28] = {
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14, 6,  61, 53, 45, 37, 29,
    21, 13, 5,  28, 20, 12, 4,
};

static int lshift[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

static int PC2[48] = {
    14, 17, 11, 24, 1, 5,
    3,  28, 15, 6,  21, 10,
    23, 19, 12, 4,  26, 8,
    16, 7,  27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

static int IP[] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

int Expansion[] =  {
    32,  1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

static int S[8][64] = {{
    /* S1 */
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
},{
    /* S2 */
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
     3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
     0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
},{
    /* S3 */
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
},{
    /* S4 */
     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
},{
    /* S5 */
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
},{
    /* S6 */
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
},{
    /* S7 */
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
},{
    /* S8 */
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
}};


static int IP1[] =  {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

void dec(int a,int c,int n,int b[]){  //fenjiedeshu cishu tiandeweizi
    for(int i = 0;i < c; i++){
        b[n] = a%2;
        a = a/2;
        n--;
    }
}

void show(int a[],int l,int n){   //array length number
    for(int i = 1;i <= l;i++){
        printf("%d",a[i-1]);
        if(i%n == 0)
            printf("  ");
    }
}

void KS()
{
    struct Kshel kcd[Nr+1] = {0};
    int temp[48] = {0};
    for(int i = 0;i < 28;i++){                //PC1
        kcd[0].c[i] = key[PC1c[i]-1];
        kcd[0].d[i] = key[PC1d[i]-1];
    }
    //show(kcd[0].c,28,28);
    //show(kcd[0].d,28,28);
    //printf("\n\n");                           //left shifts
    for(int i = 1;i <= Nr;i++){
        if(lshift[i-1] == 1){
            kcd[i].c[27] = kcd[i-1].c[0];
            kcd[i].d[27] = kcd[i-1].d[0];
            for(int j = 0;j < 27;j++){
                kcd[i].c[j] = kcd[i-1].c[j+1];
                kcd[i].d[j] = kcd[i-1].d[j+1];
            }
        }
        else{
            kcd[i].c[26] = kcd[i-1].c[0];
            kcd[i].c[27] = kcd[i-1].c[1];
            kcd[i].d[26] = kcd[i-1].d[0];
            kcd[i].d[27] = kcd[i-1].d[1];
            for(int j = 0;j < 26;j++){
                kcd[i].c[j] = kcd[i-1].c[j+2];
                kcd[i].d[j] = kcd[i-1].d[j+2];
            }
        }
        memcpy(temp,kcd[i].c,sizeof(kcd[i].c));
        memcpy(&temp[28],kcd[i].d,sizeof(kcd[i].d));
        //show(temp,56,28);
        //printf("\n");
        for(int j = 0;j < 48;j++)                   //  PC2
            Kn[i-1][j] = temp[PC2[j]-1];
    }
}

void f(int r[],int k[],int output[]){
    int temp[48] ={0};
    int x,y,n = 1;
    for(int i = 0;i < 48;i++)
        temp[i] = r[Expansion[i]-1] ^ k[i];                      //E ^ Kn
    //show(temp,48,6);
    //printf("\n");
    for(int i = 0;i < 48;i+=6){
        x = temp[i]*2 + temp[i+5];
        y = temp[i+1]*8 + temp[i+2]*4 + temp[i+3]*2 +temp[i+4];  //Sbox
        dec(S[n-1][x*16+y],4,4*n-1,output);
        n++;
     }
    //show(output,32,4);
    //printf("\n");
}

int main()
{
    char plt[8] = {'h','e','l','l','o','d','e','s'};
    int pltdec[Nk] = {0};
    int pltemp[Nk] = {0};
    int cipher[Nk] = {0};
    struct round lr[Nr+1] = {0};
    //for(int i = 0;i < 8;i++)
        //plt[i] = getchar();
    for(int i = 1;i <= Nk/8;i++)
        dec(plt[i-1],8,8*i-1,pltdec);
    memcpy(pltemp,pltdec,sizeof(pltemp));

    KS();

    for(int i = 0;i < Nk;i++)
        pltemp[i] = pltdec[IP[i]-1];

    memcpy(lr[0].l,pltemp,sizeof(lr[0].l));
    memcpy(lr[0].r,&pltemp[32],sizeof(lr[0].r));

    int output[32] ={0};
    for(int i = 1;i <= Nr;i++){
        int output[32] ={0};
        memcpy(lr[i].l,lr[i-1].r,sizeof(lr[i].l));      //Li = Ri-1
        f(lr[i-1].r,Kn[i-1],output);                    //f(Ri-1,Kn)
        for(int j = 0;j < 32;j++)
            lr[i].r[j] = lr[i-1].l[j] ^ output[j];      //Ri = Li-1 ^ f(Ri-1,Kn)

    }
    memcpy(pltemp,lr[16].r,sizeof(lr[16].r));
    memcpy(&pltemp[32],lr[16].l,sizeof(lr[16].l));

    for(int i = 0;i < Nk;i++)
        cipher[i] = pltemp[IP1[i]-1];
    show(cipher,64,8);

    for(int i = 0;i < 64;i+=8){
        int a;
        a = cipher[i]*128 + cipher[i+1]*64 + cipher[i+2]*32 + cipher[i+3]*16 + cipher[i+4]*8 + cipher[i+5]*4 + cipher[i+6]*2 + cipher[i+7];
        printf("%c ",a,a);
     }
    return 0;
}
