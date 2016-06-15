#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define thread_num 2
#define MAX_LEN 256

int number=0,count=2;//flag to say finished
sem_t sem1,sem2,sem3;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void *param){
	FILE *fp;
	char *file = (char *)param;
	int i=1;
	
	fp = fopen(file,"r");
	//printf("file is %s\n",file);
	
	while( 1 ){
	//sleep(2);//check whether this can change the status or not
	sem_wait(&sem3);
	//pthread_mutex_lock(&mutex);
	
	i=fread(&number,sizeof(int),1,fp);
	if(i==0){
	count--;
	//pthread_mutex_unlock(&mutex);
	sem_post(&sem1);
	sem_post(&sem3);
	break;
	}
	sem_post(&sem1);
	printf("%ld sent the semaphore to server\n",pthread_self());
	//pthread_mutex_unlock(&mutex);
	
	//pthread_mutex_lock(&mutex);
	if(sem_wait(&sem2)==0){
		printf("%ld number is %d",pthread_self(),number);
	}
	//pthread_mutex_unlock(&mutex);
	sem_post(&sem3);
	sleep(2);//check whether this can change the status or not
	
	}
	
	printf("thread %ld finishes\n",pthread_self());
	fclose(fp);
	pthread_exit(NULL);
	
}

int main(int argc,char *argv[]){
	
	pthread_t *th;
	int i;
	char *file;
	
	sem_init(&sem1,0,0);
	sem_init(&sem2,0,0);
	sem_init(&sem3,0,1);//to realize the mutualization of two threads
	
	file = malloc(MAX_LEN*sizeof(char));
	th = (pthread_t *)malloc(thread_num*sizeof(pthread_t));
	
	file[0] = '\0';
	sprintf(file,"%s","fv1.b");
	pthread_create(&th[0],0,thread_function,file);
	sprintf(file,"%s","fv2.b");
	pthread_create(&th[1],0,thread_function,file);
		
	while(1){
	/*If the semaphore currently has
       the value zero, then the call blocks until either it  becomes  possible
       to  perform the decrement (i.e., the semaphore value rises above zero),
       or a signal handler interrupts the call.*/
		if(sem_wait(&sem1)==0)
		{
			printf("I'm server,i got the number\n");
			//pthread_mutex_lock(&mutex);
			if(count==0){
				printf("my clients finished\n");
				break;
			}
			printf("the number is %d\n",number);
			number *= 2;
			sem_post(&sem2);
			//pthread_mutex_unlock(&mutex); 
		}
		
		}
	
	for(i=0;i<thread_num;i++){
		pthread_join(th[i],NULL);
		printf("%ld finished\n",th[i]);
	}	
		
	return 0;
}
