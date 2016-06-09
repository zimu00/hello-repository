#pragma warning(disable:4996)
#define _CRT_non_CONFORMING_SWPRINTFS
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define BUFLEN 4096
#define THREADN 4
#define MAX_SEM_NUM 50
#define TIMEOUT 2

DWORD buf[BUFLEN];
DWORD counter;
HANDLE fse;
CRITICAL_SECTION cs;
HANDLE Event;
HANDLE Event_r;
HANDLE Event_f;
DWORD counter_f;

typedef struct Threads_t{
	DWORD thId;
}threads;

DWORD WINAPI sum(LPTSTR);
DWORD WINAPI product(LPTSTR);
DWORD WINAPI factorial(LPTSTR);
DWORD WINAPI character(LPTSTR);
int createFile(LPTSTR file);

int _tmain(int argc,LPTSTR argv[]){
	HANDLE *ThreadHandle;
	threads *th;
	int i,k;
	HANDLE h;
	DWORD n,temp;

	buf[0]='\0';
	//createFile(argv[1]);
	ThreadHandle = (HANDLE *)malloc(sizeof(HANDLE)*(THREADN));
	th = (threads *)malloc(sizeof(HANDLE)*(THREADN));
	memset(buf,0,sizeof(BUFLEN));

	Event = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		TEXT("Event")
		);
	if(Event == NULL){
		_tprintf(_T("Create Event[%d] failed\n"),k);
	}

	Event_r = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		TEXT("Event_r")
		);
	if(Event_r == NULL){
		_tprintf(_T("Create Event_r failed\n"),k);
	}
	
InitializeCriticalSection(&cs);


	ThreadHandle[0] = CreateThread(0,0,(LPTHREAD_START_ROUTINE)sum,&th[0],0,&th[0].thId);
	ThreadHandle[1] = CreateThread(0,0,(LPTHREAD_START_ROUTINE)product,&th[1],0,&th[1].thId);
	ThreadHandle[2] = CreateThread(0,0,(LPTHREAD_START_ROUTINE)factorial,&th[2],0,&th[2].thId);
	ThreadHandle[3] = CreateThread(0,0,(LPTHREAD_START_ROUTINE)character,&th[3],0,&th[3].thId);

	h = CreateFile(argv[1],GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(h == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot open input file.Error:%x\n"),GetLastError());
		CloseHandle(h);
		Sleep(1000000000);
		return 1;
	}

	//read file and store into global buffer
	counter=0;
	while(ReadFile(h,&temp,sizeof(DWORD),&n,NULL)&&n>0){
	if(n!=sizeof(DWORD)){
		_ftprintf(stderr,_T("read %d error:%x\n"),sizeof(DWORD),GetLastError());
		CloseHandle(h);
		Sleep(100000000);
		return 2;
	}
	EnterCriticalSection(&cs);

	buf[counter]=temp;
	_tprintf(_T("the main thread reads number %d\n"),buf[counter]);
	counter++;
	LeaveCriticalSection(&cs);
	//add signal event here
		if(!PulseEvent(Event))
		{
			_tprintf(_T("SetEvent failed(%d)\n"),GetLastError());
			for(i=0;i<THREADN;i++){
			CloseHandle(ThreadHandle[i]);
			CloseHandle(Event);
			}
			return -1;
		}
		_tprintf(_T("SetEvent %d successful\n"),k);
	//waitfor signals of event reply
	//int result=WaitForMultipleObjects(THREADN,Event_r,TRUE,INFINITE);
	DWORD result;
	result=WaitForSingleObject(Event_r,INFINITE);
	if(result!=WAIT_OBJECT_0)
	{
		_tprintf(_T("Wait for event_r failed Error:%x\n"),GetLastError());
		for(i=0;i<THREADN;i++){
		CloseHandle(ThreadHandle[i]);
		CloseHandle(Event);
	Sleep(10000000000000);
		return -2;
	}
	_tprintf(_T("wait reply successfully\n"));
	ResetEvent(Event_r[k]);
	}	
	}
	_tprintf(_T("read finish!\n"));
	Sleep(5000);
	
	//Set Event_f to be signaled
	if(!SetEvent(Event_f))
	{
		_tprintf(_T("SetEvent failed (%d)\n"),GetLastError()); 
	}

	WaitForMultipleObjects(THREADN,ThreadHandle,TRUE,INFINITE);
	for(i=0;i<THREADN;i++){
		CloseHandle(ThreadHandle[i]);
		CloseHandle(Event[i]);
	}

	CloseHandle(Event_f);
	Sleep(10000000000000);
	return 0;
}
