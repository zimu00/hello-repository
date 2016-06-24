#include "office.h"
#include <time.h>

#define MAX_QUEUE 100


void send(Buffer *buf,Info info){
	pthread_mutex_lock(&buf->lock);
	
	while(buf->count>=buf->dim)
	pthread_cond_wait(buf->notfull,&buf->lock);
	
	buf->buffer[buf->in] = info;//...
	buf->in++;
	if(buf->in >=buf->dim)
		buf->in = 0;
	buf->count++;
	//signal that the buffer is not empty
	pthread_cond_signal(buf->notempty);
	
	pthread_mutex_unlock(&buf->lock);
}

Info receive(Buffer *b){
	Info info;
	
	//pthread_mutex_init(&b->lock,NULL);
	pthread_mutex_lock(&b->lock);
	//while(buf->in == buf->out)//
	while(b->count==0)
	//wait until the buffer is not empty
	pthread_cond_wait(b->notempty,&b->lock);
	
	info = b->buffer[b->out];
	b->count--;
	b->out++;
	if(b->out >=b->dim)
		b->out = 0;
	pthread_cond_signal(b->notfull);
		
	pthread_mutex_unlock(&b->lock);
	return info;
}

void *studentFunction(void *param){
pthread_detach(pthread_self());
	srand(time(NULL));
	int t = rand()%10+1;
	Info info;
	//int *i = (int *)param;
	//printf("i=%d\n",*i);
	int id = *(int *)param;
	printf("-------id=%d\n",id);
	info.id = id;
	info.urgent = 0;
	info.office_no = -1;
	
	//sleep(t);
	send(normal_Q,info);
	pthread_mutex_lock(&cond->lock);
	cond->normal++;
	pthread_cond_broadcast(cond->cond);
	pthread_mutex_unlock(&cond->lock);
	
	
	info = receive(answer_Q[id]);
	printf("student %d received answer from office %d\n",info.id,info.office_no);
	//service lasts 3-6s
	t = rand()%4+3;
	sleep(t);
	printf("student %d terminated after service at office %d,urgent?%d\n",info.id,info.office_no,info.urgent);
	if(info.urgent == 1){
		printf("student %d going from normal office to the special office\n",info.id);
		send(special_Q,info);
		//for special office no need to use condition 
		//and broadcast
		t = rand()%4+3;
		sleep(t);
		info = receive(answer_Q[id]);
		printf("student %d served by the special office\n",id);
		send(urgent_Q[info.office_no],info);
		pthread_mutex_lock(&cond->lock);
		cond->urgent[info.office_no]++;
		pthread_cond_broadcast(cond->cond);
		printf("urgent in office %d amount:%d\n",info.office_no,cond->urgent[info.office_no]);
		pthread_mutex_unlock(&cond->lock);
		printf("student %d going back to office %d\n",id,info.office_no);
		//service lasts 3-6s
		t = rand()%4+3;
		sleep(t);	
		info = receive(answer_Q[id]);
		printf("student %d completed at office %d\n",info.id,info.office_no);

		printf("FINALLY,student %d terminated after service at office %d\n",info.id,info.office_no);
		}
		printf("haha\n");
		pthread_mutex_lock(&num_students.lock);
		num_students.num--;
		
		printf("num of students left :%d\n",num_students.num);
		if(num_students.num==0){
		
		//pthread_exit(NULL);
		pthread_mutex_unlock(&num_students.lock);
		free(special_Q);
		free(normal_Q);
		int i;
		for(i=0;i<NUM_OFFICES;i++)
		free(urgent_Q[i]);
		for(i=0;i<k;i++)
		free(answer_Q[i]);
		exit(0);
		}
		pthread_mutex_unlock(&num_students.lock);
		
}

void *officeFunction(void *param){
	
	pthread_detach(pthread_self());
	int office_no = *(int *)param;
	Info info;
while(1){
	printf("office %d I'm servicing\n",office_no);
	pthread_mutex_lock(&cond->lock);
	printf("office %d I'm servicing,cond->urgent=%d\n",office_no,cond->urgent[office_no]);
	while( (cond->normal==0) && (cond->urgent[office_no]==0) )
	{
	//so it must need a signal to release
	pthread_cond_wait(cond->cond,&cond->lock);
	}
	
	if(cond->urgent[office_no]>0){
	cond->urgent[office_no]--;
	pthread_mutex_unlock(&cond->lock);
	
	info = receive(urgent_Q[office_no]);
	//service...
	printf("urgent received from student %d\n",info.id);
	send(answer_Q[info.id],info);
	//pthread_mutex_lock(&num_students.lock);
	/*if(num_students.num==0)
	{
	printf("office %d served all,teminate..\n",office_no);
	pthread_mutex_unlock(&num_students.lock);
	pthread_exit(NULL);
	}//need to write twice for both conditions cz both conditions may be satisfied and chech 
	else
	pthread_mutex_unlock(&num_students.lock);*/
	}
	
	
	//else{
	if(cond->normal>0){
	cond->normal--;
	pthread_mutex_unlock(&cond->lock);
	
	info = receive(normal_Q);
	
	//service....
	info.office_no = office_no;
	
	if(rand()%10<4){
		info.urgent = 1;
	}
	printf("received from student %d\n",info.id);
	send(answer_Q[info.id],info);
	//pthread_mutex_lock(&num_students.lock);
	/*if(num_students.num==0)
	{
	printf("office %d served all,teminate..\n",office_no);
	pthread_mutex_unlock(&num_students.lock);
	pthread_exit(NULL);
	}
	else{
	*/printf("---->office %d goes on servicing\n",office_no);
	//pthread_mutex_unlock(&num_students.lock);
	//}
	}
	
}
	
}

void *specialFunction(void *param){
	Info info;
	
	pthread_detach(pthread_self());
	while(1){
	info = receive(special_Q);
	printf("special received student %d\n",info.id);
	//service...
	
	send(answer_Q[info.id],info);
	
	/*pthread_mutex_lock(&num_students.lock);
	if(num_students.num==0)
	{
	printf("special office served all,teminate..\n");
	pthread_mutex_unlock(&num_students.lock);
	pthread_exit(NULL);
	}
	else
	pthread_mutex_unlock(&num_students.lock);*/
}
}
Buffer *B_init(int dim){
	Buffer *buffer;
	
	buffer =(Buffer *) malloc(sizeof(Buffer));
	pthread_mutex_init(&buffer->lock,NULL);
	buffer->notfull=malloc(sizeof(pthread_cond_t));
	buffer->notempty=malloc(sizeof(pthread_cond_t));
	
	pthread_cond_init(buffer->notfull,NULL);
	pthread_cond_init(buffer->notempty,NULL);
	buffer->in = 0;
	buffer->out = 0;
	buffer->count = 0;
	buffer->dim = dim;
	buffer->buffer = malloc(dim*sizeof(Info));
	return buffer;
	
}

Cond *cond_init(int n){
	Cond *cond;
	cond = malloc(sizeof(Cond));
	cond->cond = malloc(sizeof(pthread_cond_t));
	int i=0;
	pthread_mutex_init(&cond->lock,NULL);
	pthread_cond_init(cond->cond,NULL);
	cond->urgent = (int *)calloc(n,sizeof(int));
	for(;i<n;i++)
	  cond->urgent[i] = 0;
	cond->normal = 0;	
	
	return cond;
}

int main(int argc,char *argv[]){

if(argc!=2){
	printf("syntax error\n");
	exit(-1);
	}
	
pthread_detach(pthread_self());
int i=0,*pi,*pi2;
pthread_t th[NUM_OFFICES],th1[k],th2;

k = atoi(argv[1]);

pi2 = calloc(sizeof(int),k);
pi = calloc(sizeof(int),NUM_OFFICES);
normal_Q = B_init(MAX_QUEUE);
special_Q = B_init(MAX_QUEUE);
urgent_Q = malloc(NUM_OFFICES*sizeof(Buffer));
answer_Q = malloc(k*sizeof(Buffer));
cond = cond_init(NUM_OFFICES);
for (;i<NUM_OFFICES;i++)
   urgent_Q[i] = B_init(MAX_QUEUE);
for( i=0;i<k;i++ )
   answer_Q[i] = B_init(MAX_QUEUE);

for(i=0;i<NUM_OFFICES;i++){
   //pi = calloc(sizeof(int),1);
   pi[i] = i;
   pthread_create(&th[i],NULL,officeFunction,&pi[i]);
}

pthread_mutex_lock(&num_students.lock);
   num_students.num = k;
   printf("num_students=%d",num_students.num);
pthread_mutex_unlock(&num_students.lock);

for(i=0;i<k;i++){
  // pi = malloc(sizeof(int));
   //pi = &i;
  pi2[i] = i;
   printf("in main id=%d\n",pi2[i]);
   pthread_create(&th1[i],NULL,studentFunction,&pi2[i]);
}
pthread_create(&th2,NULL,specialFunction,NULL);
/*for(i=0;i<k;i++){
   pthread_join(th[i],NULL);
}
for(i=0;i<NUM_OFFICES;i++)
   pthread_join(th1[i],NULL);
pthread_join(th2,NULL);
*/
//free(pi);
//free(pi2);

//free everything
pthread_exit(NULL);
return 0;
}
