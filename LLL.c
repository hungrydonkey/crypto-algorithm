#include <stdio.h>
#include <string.h>
#define N 6

static inline void matshow(double matrix[][N]);
void schmidt(double base[][N],double vbase[][N],double p[][N]);
double getmu(double v[],double vo[]);

double base[N][N] = {19, 2, 32, 46, 3, 33, 15, 42, 11, 0, 3, 24, 43, 15, 0, 24, 4, 16, 20, 44, 44, 0, 18, 15, 0, 48, 35, 16, 31, 31, 48, 33, 32, 9, 1, 29};
/*
{   1,2,1,1,
	2,1,2,0,
	2,2,1,0,
	0,0,0,1,
};
*/
void initpmat(double p[][N]){
	for(int i = 0;i < N*N;i++)
		p[i/N][i%N] = (i/N == i%N) ? 1 :0;
	matshow(p);
}

static inline void arrshow(double v[],int n){
	for(int i = 0;i < n;i++)
		printf("%.10f%s",v[i],((i+1) == n) ? "\n" : " ");
}

static inline void matshow(double matrix[][N]){
	for(int i = 0;i < N*N;i++)
		printf("%.10f%s",matrix[i/N][i%N],((i+1)%N ? " " : "\n"));
}
void schmidt(double base[][N],double vbase[][N],double p[][N]){
	matshow(base);
	memcpy(vbase[0],base[0],sizeof(double)*N);
	arrshow(vbase[0],N);
	for(int i = 1;i < N;i++){
		memcpy(vbase[i],base[i],sizeof(double)*N);
		for(int j = 0;j < i;j++){
			//printf("getmu zhiqian\n");
			//arrshow(vbase[i],N);
			//puts("");
			double mu = getmu(base[i],vbase[j]);
			p[i][j] = mu;
			printf("mu(%d,%d) is %.10f\n\n",i+1,j+1,mu);
			for(int n = 0;n < N;n++){
				vbase[i][n] = vbase[i][n] - mu*vbase[j][n]; 
			}
			//arrshow(vbase[i],N);
			//puts("");
		}
		matshow(vbase);
		puts("");
	}
	matshow(p);
}

double getmu(double v[],double vo[]){
	double vovo = 0,vvo = 0;
	for(int i = 0;i < N;i++){
		vvo  = v[i] * vo[i] + vvo;
		vovo = vo[i] * vo[i] + vovo;
	}
	printf("%f %f\n",vvo,vovo);
	return vvo/vovo;
}

int main(void){
	double vbase[N][N] = {0};
	double pmatrix[N][N];
	initpmat(pmatrix);
	schmidt(base,vbase,pmatrix);

}
