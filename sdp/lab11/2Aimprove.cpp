#pragma warning(disable:4996)
#define _CRT_NON_CONFORMING_SWPRINTFS
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define BUFFER 200
#define MAX_ITERATION 5

INT N;
INT T;
DWORD global_message_id = 0;
CRITICAL_SECTION cs_m;//to protect messages;

typedef struct Messages
{
	DWORD thId;
	TCHAR content[BUFFER];
}Message,*LPMESSAGE;

typedef struct Queue
{
	LPMESSAGE message;
	CRITICAL_SECTION cs;
	DWORD counter;
	CONDITION_VARIABLE full;
	CONDITION_VARIABLE empty;
	DWORD insert_p;
	DWORD remove_p;
	DWORD dimension;
}queue,*LPQUEUE;

typedef struct Threads
{
	DWORD thId;
	LPQUEUE q;
}Threads_t,*pThreads;

LPQUEUE initialQueue(DWORD);
VOID destroyQueue(LPQUEUE q);
DWORD WINAPI product(LPVOID);
DWORD WINAPI consume(LPVOID);
VOID enqueue(LPQUEUE q,Message m);
Message dequeue(LPQUEUE q);
Message writeMessage(LPTSTR content);

int _tmain(int argc,LPTSTR argv[]){
	HANDLE *ThreadHandle;
	Threads_t *thread;
	INT count;
	INT i;
	INT P,C;
	LPQUEUE products; 
	products = NULL;

	P=_tstoi(argv[1]);
	C=_tstoi(argv[2]);
	count = P + C;
	N = _tstoi(argv[3]);
	T = _tstoi(argv[4]);
	_tprintf(_T("N=%d,T=%d\n"),N,T);
	_tprintf(_T("there are %d threads will be created\n"),count);

	InitializeCriticalSection(&cs_m);
	products=initialQueue(N);
	

	ThreadHandle = (HANDLE *)malloc(sizeof(HANDLE)*count);
	thread = (Threads_t *)malloc(sizeof(Threads_t *)*count);

	for(i=0;i<P;i++)
	{
		thread[i].q = products;
		ThreadHandle[i] = CreateThread(0,0,(LPTHREAD_START_ROUTINE)product,&thread[i],0,&thread[i].thId);
	}
	for(;i<count;i++)
	{
		thread[i].q = products;
		ThreadHandle[i] = CreateThread(0,0,(LPTHREAD_START_ROUTINE)consume,&thread[i],0,&thread[i].thId);
	}


	WaitForMultipleObjects(P,ThreadHandle,TRUE,INFINITE);
	WaitForMultipleObjects(C,ThreadHandle,TRUE,INFINITE);
	for(i=0;i<count;i++)
	{
		CloseHandle(ThreadHandle[i]);
	}

	DeleteCriticalSection(&cs_m);
	destroyQueue(products);
	free(thread);
}

DWORD WINAPI product(LPVOID param){
	pThreads th = (pThreads)param;
	Message m;
	srand(time(NULL));
	DWORD n,j;
	DWORD i=0;

	while(i<MAX_ITERATION){
	//generate random number at random time interval
	n=1000*rand()%(T+1);
	Sleep(n);
	j = rand()%10;
	_tprintf(_T("j is %d\n"),j);
	_stprintf(m.content,_T("message %d produced by thread %d\n"),j,th->thId);
	m=writeMessage(m.content);
	enqueue(th->q,m);
	_tprintf(_T("producer %d sent one message to the queue\n"),th->thId);
	i++;
	}
		_tprintf(_T("producer %d finishes!\n---------"),th->thId);
	Sleep(1000000000);
	return 0;
}

DWORD WINAPI consume(LPVOID param){
	pThreads th = (pThreads)param;

	Message m;
	srand(time(NULL));
	INT n;	
	DWORD i=0;

	while(i<MAX_ITERATION){
	//generate random number at random time interval
	n=1000*rand()%(T+1);
	Sleep(n);

	//dequeue
	 m=dequeue(th->q);
	_tprintf(_T("consumer %d consumes product  content is %s\n"),th->thId,m.content);
	i++;
	}
	_tprintf(_T("consumer %d finishes!\n---------"),th->thId);
	Sleep(1000000000);
	return 0;
}
LPQUEUE initialQueue(DWORD dimenstion){
	LPQUEUE q=NULL;
	q=(LPQUEUE)calloc(1,sizeof(Queue));

	_tprintf(_T("sizeof(LPMESSAGE=%d,sizeof(Message)=%d\n"),sizeof(LPMESSAGE),sizeof(Message));
	q->counter = 0;
	q->insert_p = 0;
	q->remove_p = 0;
	q->dimension = dimenstion;
	//q->message->content[0]=0;
	q->message = (LPMESSAGE)calloc(dimenstion,sizeof(Message));//size of LPMESSAGE 

	InitializeConditionVariable(&q->full);
	InitializeConditionVariable(&q->empty);
	InitializeCriticalSection(&q->cs);
	return q;
}

VOID enqueue(LPQUEUE q,Message m){
	if(q == NULL){
		_ftprintf(stderr,_T("dequeue called on a NULL queue\n"));
		m.thId = 0;
		return;
	}

	while( q->counter ==q->dimension )
	{
		_tprintf(_T("the queue is full now\n"));
		SleepConditionVariableCS(&q->full,&q->cs,INFINITE);
	}
	EnterCriticalSection(&q->cs);
	q->message[q->insert_p++]=m;
	_tprintf(_T("---message %d enters into queue\n"),m.thId);
	q->counter++;
	q->insert_p %= q->dimension;
	WakeConditionVariable(&q->empty);//wake up a thread who is waiting cz of empty
	LeaveCriticalSection(&q->cs);
	_tprintf(_T("%d I left critical section\n"),GetCurrentThreadId());
	
}

Message dequeue(LPQUEUE q){
	Message m;

	if(q == NULL){
		_ftprintf(stderr,_T("dequeue called on a NULL queue\n"));
		m.thId = 0;
		return m;
	}

	EnterCriticalSection(&q->cs);
	while (q->counter == 0)
	{
		_tprintf(_T("no messages now,sleep.....\n"));
		SleepConditionVariableCS(&q->empty,&q->cs,INFINITE);
	}
	assert(q->counter>0);
	_tprintf(_T("has messages,dequeuing now\n"));
	q->counter--;
	m=q->message[q->remove_p++];
	q->remove_p %= q->dimension;
	WakeConditionVariable(&q->full);
	LeaveCriticalSection(&q->cs);
	return m;
}

Message writeMessage(LPTSTR content){
	Message m;
	EnterCriticalSection(&cs_m);
	m.thId=global_message_id++;
	_tprintf(_T("%d creates messages with id=%d"),GetCurrentThreadId(),m.thId);
	_stprintf(m.content,_T("%s"),content);
	LeaveCriticalSection(&cs_m);
	return m;
}

VOID destroyQueue(LPQUEUE q){
	if (q == NULL){
		_ftprintf(stderr,_T("Destroy a NULL queue error!\n"));
		return;
	}
	else{
		assert(q->message);
		free(q->message);
		DeleteCriticalSection(&q->cs);
		free(q);
	}
}