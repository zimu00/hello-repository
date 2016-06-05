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

INT N;
INT T;
DWORD global_message_id = 0;
CRITICAL_SECTION cs_m;//to protect messages;

typedef struct Queue
{
	LPMESSAGE message;
	CRITICAL_SECTION cs;
	DWORD counter;
	CONDITION_VARIABLE full;
	CONDITION_VARIABLE empty;
	CONDITION_VARIABLE MEp;
	CONDITION_VARIABLE MEc;
	DWORD insert_p;
	DWORD remove_p;
	DWORD dimension;
}queue,*LPQUEUE;

typedef struct Threads
{
	DWORD thId;
	LPQUEUE q;
}Threads_t;

typedef struct Messages
{
	DWORD thId;
	TCHAR content[BUFFER];
}Message,*LPMESSAGE;

LPQUEUE initialQueue(DWORD);
DWORD WINAPI product(LPVOID);
DWORD WINAPI consume(LPVOID);

int _tmain(int argc,LPTSTR argv[]){
	HANDLE *ThreadHandle;
	Threads_t *thread;
	INT count;
	INT i;
	INT P,C;
	LPQUEUE products; 

	P=_tstoi(argv[1]);
	C=_tstoi(argv[1]);
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
		ThreadHandle[i] = CreateThread(0,0,(LPTHREAD_START_ROUTINE)consume,&thread[i],0,&thread[i].thId);
	}


	WaitForMultipleObjects(count,ThreadHandle,TRUE,INFINITE);
	for(i=0;i<count;i++)
	{
		CloseHandle(ThreadHandle[i]);
	}

	DeleteCriticalSection(&cs_m);
	free(products);
	free(thread);
}

DWORD WINAPI product(LPVOID param){
	Threads_t *th = (Threads_t *)param;
	Message m;
	srand(time(NULL));
	int n,j;

	while(1){
	//generate random number at random time interval
	n=rand()%(T+1);
	Sleep(n);
	j = rand()%10;
	m.content

	WaitForSingleObject(empty,INFINITE);
	//_tprintf(_T("%d the semaphore of MEp amount is %d\n"),th->thId,MEp);
	WaitForSingleObject(MEp,INFINITE);
	//wait for mutex
	WaitForSingleObject(mutex,INFINITE);
	_tprintf(_T("\n%d has got the mutex\n"),th->thId);
	//enqueue
	products[counter%N]=m;
	counter++;
	_tprintf(_T("%d produced product %d,now has %d products\n"),th->thId,m,counter);
	if(!ReleaseMutex(mutex))
	{
		_ftprintf(stderr,_T("%d Release failed.Error:%x\n"),th->thId,GetLastError());
		Sleep(1000000000);
		return 9;
	}
	else 
		_tprintf(_T("%d released the mutex\n"),th->thId);
	//signal MEp
	if (!ReleaseSemaphore(
		MEp,
		1,
		NULL
		))
	{
		_tprintf(_T("----->%d thread ReleaseMEp error:%d\n"),th->thId,GetLastError());
	}
	else
	{
		_tprintf(_T("----->%d thread have release MEp\n"),th->thId);
	}
	//signal full
	if (!ReleaseSemaphore(
		full,
		1,
		NULL
		))
	{
		_tprintf(_T("----->%d thread Releasefull error:%d\n"),th->thId,GetLastError());
	}
	else
	{
		_tprintf(_T("----->%d thread have release full\n"),th->thId);
	}
	}
	Sleep(1000000000);
	return 0;
}

DWORD WINAPI consume(LPVOID param){
	Threads_t *th = (Threads_t *)param;

	//Message m;
	srand(time(NULL));
	int n,m;

	while(1){
	//generate random number at random time interval
	n=rand()%(T+1);
	Sleep(n);
	m = rand()%10;

	WaitForSingleObject(full,INFINITE);
	WaitForSingleObject(MEc,INFINITE);
	//wait for mutex
	WaitForSingleObject(mutex,INFINITE);
	_tprintf(_T("\n%d consumer has got the mutex\n"),th->thId);
	//dequeue
	counter--;//??
	m=products[counter%N];
	_tprintf(_T("%d Now the counter value is %d\n"),th->thId,counter);

	if(!ReleaseMutex(mutex))
	{
		_ftprintf(stderr,_T("%d consumer Release failed.Error:%x\n"),th->thId,GetLastError());
		Sleep(1000000000);
		return 9;
	}
	else 
		_tprintf(_T("%d released the mutex\n"),th->thId);
	//signal MEp
	if (!ReleaseSemaphore(
		MEc,
		1,
		NULL
		))
	{
		_tprintf(_T("----->%d thread ReleaseMEp error:%d\n"),th->thId,GetLastError());
	}
	else
	{
		_tprintf(_T("----->%d thread have release MEp\n"),th->thId);
	}
	//signal full
	if (!ReleaseSemaphore(
		empty,
		1,
		NULL
		))
	{
		_tprintf(_T("----->%d thread Releasefull error:%d\n"),th->thId,GetLastError());
	}
	else
	{
		_tprintf(_T("----->%d thread have released full\n"),th->thId);
	}
	_tprintf(_T("consumer %d consumes product %d"),th->thId,m);
	}
	Sleep(1000000000);
	return 0;
}
LPQUEUE initialQueue(DWORD dimenstion){
	LPQUEUE q=NULL;
	q=(LPQUEUE)calloc(1,sizeof(LPQUEUE));

	q->counter = 0;
	q->insert_p = 0;
	q->remove_p = 0;
	q->dimension = dimenstion;
	q->message = (LPMESSAGE)calloc(dimenstion,sizeof(LPMESSAGE));//size of LPMESSAGE 

	InitializeConditionVariable(&q->full);
	InitializeConditionVariable(&q->empty);
	InitializeConditionVariable(&q->MEc);
	InitializeConditionVariable(&q->MEp);

	return q;
}

VOID enqueue(LPQUEUE q,Message m){
	if(q == NULL){
		_ftprintf(stderr,_T("dequeue called on a NULL queue\n"));
		m.thId = 0;
		return m;
	}

	while(q->counter ==q->dimension)
	{
		_tprintf(_T("the queue is full now\n");
		SleepConditionVariableCS(&q->full,&q->cs,INFINITE);
	}
	EnterCriticalSection(&q->cs);
	q->message[q->insert_p++]=m;
	q->counter++;
	q->insert_p %= q->dimension;
	WakeConditionVariable(&q->empty);//wake up a thread who is waiting cz of empty
	LeaveCriticalSection(&q->cs);
	_tprintf(_T("---message %d enters into queue\n"),m.thId);
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