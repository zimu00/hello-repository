#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

void sort(int *v,int n){

	int i,j;
	int temp;
	
	for(i=0;i<n-1;i++)
	   for(j=0;j<n-1;j++)
	   {
	   	if(v[j]>v[j+1]){
			temp = v[j+1];
			v[j+1] = v[j];
			v[j] = temp;
			}	   		
	   }
	   //check
	   printf("\ncheck the sorting result:\n");
	   for(i=0;i<n;i++)
	   	printf("%d ",v[i]);
}


int main(int argc,char *argv[]){
	
	int n1 = atoi(argv[1]);
	int n2 = atoi(argv[2]);
	int *v1,*v2;
	int a1,a2;
	int i;
	int fp1,fp2,fp11,fp22;
	FILE *fp3,*fp4,*fp5,*fp6;
	srand(time(NULL));
	
	v1 = (int *)malloc(n1*sizeof(int));
	v2 = (int *)malloc(n2*sizeof(int));
	fp1 = open("fv1.txt",O_CREAT | O_WRONLY | O_RDONLY, S_IRUSR | S_IWUSR);
	fp2 = open("fv2.txt",O_CREAT | O_WRONLY | O_RDONLY, S_IRUSR | S_IWUSR);
	fp11 = open("fv1.txt",O_RDONLY);
	fp22 = open("fv2.txt",O_RDONLY);
	fp3 = fopen("fv1.b","w");
	fp4 = fopen("fv2.b","w");
	fp5 = fopen("fv1.b","r");
	fp6 = fopen("fv2.b","r");
	
	printf("v1 unordered\n");
	for( i=0;i<n1;i++ )
	{
		v1[i] = rand()%101+10;
		printf("%d ",v1[i]);
	}
	sort(v1,n1);
	
	printf("\nv1:\n");
	for( i=0;i<n1;i++ )
	{
		write(fp1,&v1[i],sizeof(int));
		//check result of writing
		read(fp11,&a1,sizeof(int));
		printf("%d ",a1);
		
		fwrite(&v1[i],sizeof(int),1,fp3);
		fread(&a1,sizeof(int),1,fp4);
		printf("binary:%d ",a1);
	}
	
	printf("\n");
	
	printf("v2 unordered\n");
	for( i=0;i<n2;i++ )
	{
		v2[i] = rand()%101+20;
		printf("%d ",v2[i]);
		
	}
	sort(v2,n2);
	
	printf("v2:\n");
	for( i=0;i<n2;i++ )
	{
		write(fp2,&v2[i],sizeof(int));
		//check result of writing
		read(fp22,&a2,sizeof(int));
		printf("%d ",a2);
		
		fwrite(&v2[i],sizeof(int),1,fp5);
		fread(&a2,sizeof(int),1,fp6);
		printf("binary:%d ",a2);
	
	}

	
	
	close(fp1);
	close(fp2);
	close(fp11);
	close(fp22);	
	return 0;
}
