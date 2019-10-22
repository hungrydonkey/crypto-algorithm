#include <stdio.h>
#include <string.h>
#define N 3

void show(double matrix[][N]);
void schmidt(double base[][N],double vbase[][N]);
double getmu(double v[],double vo[]);

double base[N][N] = 
{   1,2,1,//0,
	2,1,2,//0,
	2,2,1,//0,
	//0,0,0,1,
};
static inline void cpyarr(double s[],double d[],int a){
	for(int i = 0;i < a;i++)
		d[i] = s[i];
}
static inline void arrshow(double v[],int n){
	for(int i = 0;i < n;i++)
		printf("%f%s",v[i],((i+1) == n) ? "\n" : " ");
}

static inline void matshow(double matrix[][N]){
	for(int i = 0;i < N*N;i++)
		printf("%f%s",matrix[i/N][i%N],((i+1)%N ? " " : "\n"));
}
void schmidt(double base[][N],double vbase[][N]){
	//double vtemp[N];
	//printf("%lu %lu\n",sizeof(base),sizeof(double));
	matshow(base);
	cpyarr(base[0],vbase[0],N);
	arrshow(vbase[0],N);
	for(int i = 1;i < N;i++){
		//cpyarr(base[i],vtemp,N);
		for(int j = 0;j < i;j++){
			double mu = getmu(base[i],vbase[j]);
			printf("mu is %f\n",mu);
			for(int n = 0;n < N;n++){
				vbase[i][n] = base[i][n] - mu*vbase[j][n]; 
			}
			//arrshow(vtemp,N);
			arrshow(vbase[i],N);
			//break;
		}
		//break;
	}
	matshow(vbase);
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
	schmidt(base,vbase);

}
