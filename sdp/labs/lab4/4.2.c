//only last thread checks
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdio.h>//perror
#include<math.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>

typedef struct{
	int left;
	int right;
	int id;
}thread;

void *swap(void *param);
void checkBoundary(thread *th);

thread *th;
int finish=0;
int n;
int *buf;
pthread_mutex_t lock;
int *flag;
sem_t se,*se1;

int main(int argc,char *argv[]){

if(argc!=2)
{
	printf("paramete error\n");
	exit(-1);
}

int i=0;
pthread_t *pth;
int fp;
int *pi;
struct stat stat_buf;
char c,*paddr;
int len;

pthread_mutex_init(&lock,NULL);
fp = open(argv[1],O_RDWR);
fstat(fp,&stat_buf);
len = stat_buf.st_size/(sizeof(int));
double x = log10(len);
n = ceil(x);
printf("%d,%d\n",len,n);
pth = malloc(n*sizeof(pthread_t));
pi = malloc(n*sizeof(int));
th = malloc(n*sizeof(thread));
//se = malloc(n/2*sizeof(sem_t));
se1 = malloc((n-1)*sizeof(sem_t));
flag = (int *)calloc(sizeof(int),n/2);

paddr = mmap( (caddr_t)0,len,PROT_READ | PROT_WRITE,MAP_SHARED,(int )fp,0 );
buf = (int *)paddr;
sem_init(&se,0,0);
close(fp);

printf("len=%d the original array is :\n",len);
for(;i<len;i++)
printf("%d ",buf[i]);

for(i=0;i<n;i++)
{
	th[i].left = i*len/n;
	printf("%d ",th[i].left);
	sleep(3);
	if(len>=(i+1)*len/n)
	th[i].right = th[i].left +len/n;
	else
	th[i].right = len;
	
	th[i].id = i;
	pth[i] = pthread_create(&pth[i],NULL,swap,&th[i]);
}
sleep(1000000);
pthread_exit(NULL);
//sleep(10000000000);
//return 0;
}

void *swap(void *param){

thread *th = (thread *)param;
int i,j;
int tmp;

pthread_detach(pthread_self());
printf("\nI'm thread %d before sorting\n",th->id);
for(i = th->left;i<th->right;i++){
	printf("%d ",buf[i]);
}

while(1){
for(i=th->left;i<th->right;i++){
	for(j=th->left;j<th->right;j++){
		if(buf[j]>buf[j+1]){
			tmp = buf[j];
			buf[j]=buf[j+1];
			buf[j+1]=tmp;
		}	
	}
}
printf("\nthread %d finishes swapping,%d,%d\n",th->id,th->left,th->right);
for(i = th->left;i<th->right;i++){
	printf("%d ",buf[i]);
}

pthread_mutex_lock(&lock);
if(th->id==0)
flag[th->id]++;
else{
flag[th->id/2]++;
flag[th->id/2+1]++;
}
pthread_mutex_unlock(&lock);

if(flag[th->id/2]==2){
	sem_post(&se);
	printf("\n thread%d sent semaphore\n",th->id);

	if(th->id==n-1){
//check
	checkBoundary(th);
	}

	sem_wait(&se1[th->id/2]);

	}
	
	if(flag[th->id/2+1]==2){
//sem_post(se[th->id/2]);
	sem_post(&se);
	if(th->id==n-1){
//check
	checkBoundary(th);
	}
	sem_wait(&se1[th->id/2+1]);

	}
}
}

void checkBoundary(thread *th){

	int i;
	int tmp;
	int finish=0;	
	int time=0;
	
	while(1){
	if(time==n-1 && finish==n-1)
	{
		printf("all are sorted\n");
		for(i=0;i<th[n-1].left;i++)
			printf("%d ",buf[i]);
		exit(0);	
	}
	
	sem_wait(&se);
	time++;
	
	pthread_mutex_lock(&lock);
	for(i=0;i<n-1;i++){
	
		if(flag[i]==2)
			break;
	
	}
	pthread_mutex_unlock(&lock);
	
	if(buf[th[i].right]>buf[th[i+1].left])
	{
		tmp = buf[th[i].right];
		buf[th[i].right] = buf[th[i+1].left];
		buf[th[i+1].left] = tmp;
	}
	else
		finish++;
	
	if(th->id==i-1){
		sem_post(&se1[i]);
		swap(th);
	}
	sem_post(&se1[i]);
	sem_post(&se1[i]);
	
	}
}
	
