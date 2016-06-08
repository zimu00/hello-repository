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

DWORD buf[BUFLEN];
HANDLE semaphore;
CRITICAL_SECTION cs;

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
	int i;
	HANDLE h;
	DWORD n,temp;

	createFile(argv[1]);
	ThreadHandle = (HANDLE *)malloc(sizeof(HANDLE)*(THREADN));
	th = (threads *)malloc(sizeof(HANDLE)*(THREADN));
	memset(buf,0,sizeof(BUFLEN));
	semaphore = CreateSemaphore(NULL,1,MAX_SEM_NUM,NULL);
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
	i=0;
	while(ReadFile(h,&temp,sizeof(DWORD),&n,NULL)&&n>0){
	if(n!=sizeof(DWORD)){
		_ftprintf(stderr,_T("read %d error:%x\n"),i*sizeof(DWORD),GetLastError());
		CloseHandle(h);
		Sleep(100000000);
		return 2;
	}
	buf[i]=temp;
	//add sending semaphore here
	}
	//add sending final semaphore here

	WaitForMultipleObjects(THREADN,ThreadHandle,TRUE,INFINITE);
	for(i=0;i<THREADN;i++){
		CloseHandle(ThreadHandle[i]);
	}

	return 0;
}

int createFile(LPTSTR file){
	_tprintf(_T("Please input integer numbers(separated with space and terminated with ctrl-d)"));
	int i=0;
	HANDLE hIn,hOut;
	DWORD nWrt,nRd;

	while(buf[i]!=EOF)
	{
		_tscanf(_T("%d"),buf[i]);
		++i;
	}

	hIn = CreateFile(file,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hIn == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot open input file.Error:%x\n"),GetLastError());
		CloseHandle(hIn);
		Sleep(1000000000);
		return 1;
	}
	WriteFile(hIn,buf,i*sizeof(DWORD),&nWrt,NULL);
	if(nWrt!=i*sizeof(DWORD)){
		_ftprintf(stderr,_T("write %d error:%x\n"),i*sizeof(DWORD),GetLastError());
		CloseHandle(hIn);
		Sleep(100000000);
		return 2;
	}
	CloseHandle(hIn);
	//check the file content
	memset(buf,0,BUFLEN);
	hOut = CreateFile(file,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hOut == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot open input file.Error:%x\n"),GetLastError());
		CloseHandle(hOut);
		Sleep(1000000000);
		return 1;
	}
	ReadFile(hOut,buf,i*sizeof(DWORD),&nRd,NULL);
	if(nRd!=i*sizeof(DWORD)){
		_ftprintf(stderr,_T("read %d error:%x\n"),i*sizeof(DWORD),GetLastError());
		CloseHandle(hOut);
		Sleep(100000000);
		return 2;
	}
	for(i=0;i<BUFLEN;i++)
	{
		_tprintf(_T("%d "),buf[i]);
	}
	CloseHandle(hOut);
}

DWORD WINAPI sum(LPTSTR param)
{
	threads *th = (threads *)param;
	DWORD buf_sum[BUFLEN];
	int i,j=0;

	//repeat till receive final semaphore,so add check final semaphore here
	while(){
	EnterCriticalSection(&cs);
	i = _tcslen(buf);
	while(j<i)
	{
	//do the summation here
	}
	LeaveCriticalSection(&cs);
	j=i;
	}
}
DWORD WINAPI product(LPTSTR)
{

}
DWORD WINAPI factorial(LPTSTR)
{

}
DWORD WINAPI character(LPTSTR){

}
