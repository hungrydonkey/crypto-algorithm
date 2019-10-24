#include <stdio.h>
#include <string.h>
#define N 6
#define SZ sizeof(double)
typedef double (*p)[N]; 
static inline void matshow(double matrix[][N]);
static void schmidt(double base[][N],double vbase[][N],double p[][N]);
static double getmu(double v[],double vo[]);
static double (*lll(void))[N]; //lll(void);

double base[N][N] = {19, 2, 32, 46, 3, 33, 15, 42, 11, 0, 3, 24, 43, 15, 0, 24, 4, 16, 20, 44, 44, 0, 18, 15, 0, 48, 35, 16, 31, 31, 48, 33, 32, 9, 1, 29};
/*
{   1,2,1,1,
	2,1,2,0,
	2,2,1,0,
	0,0,0,1,
};
*/
double vbase[N][N];
double pmatrix[N][N];

void initpmat(double p[][N]){
	for(int i = 0;i < N*N;i++)
		p[i/N][i%N] = (i/N == i%N) ? 1 :0;
	//matshow(p);
}

static inline void arrshow(double v[],int n){
	for(int i = 0;i < n;i++)
		printf("%15.10f%s",v[i],((i+1) == n) ? "\n" : " ");
}

static inline void matshow(double matrix[][N]){
	for(int i = 0;i < N*N;i++)
		printf("%15.10f%s",matrix[i/N][i%N],((i+1)%N ? " " : "\n"));
}
static void schmidt(double base[][N],double vbase[][N],double p[][N]){
	//matshow(base);
	memcpy(vbase[0],base[0],sizeof(double)*N);
	//arrshow(vbase[0],N);
	for(int i = 1;i < N;i++){
		memcpy(vbase[i],base[i],sizeof(double)*N);
		for(int j = 0;j < i;j++){
			//printf("getmu zhiqian\n");
			//arrshow(vbase[i],N);
			//puts("");
			double mu = getmu(base[i],vbase[j]);
			p[i][j] = mu;
			//printf("mu(%d,%d) is %.10f\n\n",i+1,j+1,mu);
			for(int n = 0;n < N;n++){
				vbase[i][n] = vbase[i][n] - mu*vbase[j][n]; 
			}
			//arrshow(vbase[i],N);
			//puts("");
		}
		//matshow(vbase);
		//puts("");
	}
	//matshow(p);
}

static double getmu(double v[],double vo[]){
	double vovo = 0,vvo = 0;
	for(int i = 0;i < N;i++){
		vvo  = v[i] * vo[i] + vvo;
		vovo = vo[i] * vo[i] + vovo;
	}
	//printf("%f %f\n",vvo,vovo);
	return vvo/vovo;
}
static double getmo(double v[]){
	int mo = 0;
	for(int i = 0;i <  N;i++)
		mo += v[i] * v[i];
	return mo;	
}
static int intmu(double mu){
	return mu < 0? (int)(mu + -0.5) : (int)(mu + 0.5);
}

static double (*lll(void))[N]{
#define SQURE(x) ((x) * (x))
	int k = 1;
	//double imatrix[N][N] = {0};
	double tempmat[N][N];
	static double lbase[N][N];
	double lvbase[N][N];
	initpmat(tempmat);
	memcpy(lbase,base,sizeof(double)*N*N);
	memcpy(lvbase,vbase,sizeof(double)*N*N);
	while(k < N){
		//printf("%d ",k+1);
		for(int i = k-1;i > 0;i--){
			double mu = getmu(lbase[k],lvbase[i]);
			for(int j = 0;j < N;j++)
				lbase[k][j] = lbase[k][j] - intmu(mu) * lbase[i][j]; 
		}
		schmidt(lbase,lvbase,tempmat);
		double af = 3/4 - SQURE(getmu(lbase[k],lvbase[k-1]));
		if(getmo(lvbase[k]) >= af * SQURE(getmo(lvbase[k-1])))
			k++;
		else{
			double tempv[N] = {0};
			memcpy(tempv,lbase[k],SZ*N);
			memcpy(lbase[k],lbase[k-1],SZ*N);
			memcpy(lbase[k-1],tempv,SZ*N);
			k = k-1 < 2 ? 2 : k-1;
			schmidt(lbase,lvbase,tempmat);
			puts("swap");
		}
		printf("%d ",k+1);
	}
	puts("");
	return lbase;
}

int main(void){
	initpmat(pmatrix);
	schmidt(base,vbase,pmatrix);
	//lll(base,vbase);
	printf("%d %d %d\n",intmu(-0.7),intmu(-2.1),intmu(0.4999999999));
	matshow(lll());
}

