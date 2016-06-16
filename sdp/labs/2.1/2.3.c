#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

float **mat;
float *v1;
float *v2;
int row=0;
float *v3;
float result=0;

void *product(void *param){
	int *n = (int *)param;
	int i =0;
	
	pthread_detach(pthread_self());
	for(;i<*n;i++){
	    v3[i] += v1[i]*mat[row][i];
	}
	row++;
	if(row == i){
	    printf("I'm last line\n");
	    for(i=0;i<*n;i++)
	    result += v3[i]*v2[i];
	    printf("result is %f\n",result);
	    //release memory
	    free(v1);
	    free(v2);
	    free(v3);
	    free(mat);
	}
	pthread_exit(NULL);
}


int main(int argc,char *argv[]){
	int i,j;
	pthread_t *th; 
	srand(time(NULL));
	int sign_time,sign=1;
	int n;
	
	if(argc!=2){
	printf("syntax error\n");
	exit(-1);
	}
	
	n = atoi(argv[1]);
	int *p=&n;//??
	th = (pthread_t *)malloc(n*sizeof(pthread_t));
	mat = (float **)malloc(n*n*sizeof(float));
	v1 = (float *)malloc(n*sizeof(float));
	v2 = (float *)malloc(n*sizeof(float));
	v3 = (float *)calloc(n,sizeof(float));
	
	for(i=0;i<n;i++)
	mat[i] = (float *)malloc(n*sizeof(float));

	for(i=0;i<n;i++){
	sign_time = rand()%2+1;
	 for(j=0;j<sign_time;j++)
	 sign *= (-1);
	v1[i] = sign*rand()%6*0.1;
	v2[i] = sign*rand()%6*0.1;
	 for(j=0;j<n;j++)
	{ mat[i][j]=sign*rand()%6*0.1;
	 printf("%f ",mat[i][j]);
	}printf("\n");
	}
	
	for(i=0;i<n;i++)
	pthread_create(&th[i],NULL,product,(void *)p);

	pthread_detach(pthread_self());
		
	pthread_exit(NULL);
		
	return 0;
}
