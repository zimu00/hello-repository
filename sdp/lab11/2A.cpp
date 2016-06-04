#pragma warning(disable:4996)
#define _CRT_NON_CONFORMING_SWPRINTFS
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER 200

INT N;
INT T;

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
	Threads_t *thread = (Threads_t *)param;


	return 0;
}

DWORD WINAPI consume(LPVOID param){
	Threads_t *thread = (Threads_t *)param;

	return 0;
}