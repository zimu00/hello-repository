#pragma warning(disable:4996)
#define _CRT_NON_CONFORMING_SWPRINTFS
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER 200

INT N;
INT T;
HANDLE full;
HANDLE empty;
HANDLE MEp;
HANDLE MEc;
HANDLE mutex;
INT counter=0;
INT products[BUFFER];

typedef struct Threads
{
	DWORD thId;

}Threads_t;

DWORD WINAPI product(LPVOID);
DWORD WINAPI consume(LPVOID);

int _tmain(int argc,LPTSTR argv[]){
	HANDLE *ThreadHandle;
	Threads_t *thread;
	INT count;
	INT i;
	INT P,C;

	P=_tstoi(argv[1]);
	C=_tstoi(argv[1]);
	count = P + C;
	N = _tstoi(argv[3]);
	T = _tstoi(argv[4]);
	_tprintf(_T("N=%d,T=%d\n"),N,T);
	_tprintf(_T("there are %d threads will be created\n"),count);

	//Create a mutex with no initial owner
	mutex = CreateMutex(
			NULL, // default security attributes
			FALSE,// initially not owned
			NULL// unnamed mutex
		);
	full = CreateSemaphore(
		NULL,
		0,
		N,
		NULL
		);
	empty = CreateSemaphore(
		NULL,
		N,
		N,
		NULL
		);
	MEp = CreateSemaphore(
		NULL,
		1,
		1,
		NULL
		);
	MEc = CreateSemaphore(
		NULL,
		1,
		1,
		NULL
		);
	ThreadHandle = (HANDLE *)malloc(sizeof(HANDLE)*count);
	thread = (Threads_t *)malloc(sizeof(Threads_t *)*count);

	for(i=0;i<P;i++)
	{
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

	free(thread);
}

DWORD WINAPI product(LPVOID param){
	Threads_t *th = (Threads_t *)param;
	//Message m;
	srand(time(NULL));
	int n,m;

	while(1){
	//generate random number at random time interval
	n=rand()%(T+1);
	Sleep(n);
	m = rand()%10;

	WaitForSingleObject(empty,INFINITE);
	WaitForSingleObject(MEp,INFINITE);
	//wait for mutex
	WaitForSingleObject(mutex,INFINITE);
	_tprintf(_T("\n%d has got the mutex\n"),th->thId);
	//enqueue
	products[counter%N]=m;
	if(!ReleaseMutex(mutex))
	{
		_ftprintf(stderr,_T("%d Release failed.Error:%x\n"),th->thId,GetLastError());
		return 9;
	}
	else 
		_tprintf(_T("%d released the mutex\n"),th->thId);
	}
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
	m=products[counter%N];
	counter--;//??

	if(!ReleaseMutex(mutex))
	{
		_ftprintf(stderr,_T("%d consumer Release failed.Error:%x\n"),th->thId,GetLastError());
		return 9;
	}
	else 
		_tprintf(_T("%d released the mutex\n"),th->thId);
	}
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
		_tprintf(_T("----->%d thread have release full\n"),th->thId);
	}
	_tprintf(_T("consumer %d consumes product %d"),th->thId,m);

	return 0;
}