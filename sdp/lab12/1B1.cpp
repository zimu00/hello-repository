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
volatile DWORD counter,counter1=0;
HANDLE fse;
CRITICAL_SECTION cs;
HANDLE Event;
HANDLE Event_r;

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
		_tprintf(_T("Create Event failed\n"));
	}

	Event_r = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		TEXT("Event_r")
		);
	if(Event_r == NULL){
		_tprintf(_T("Create Event_r failed\n"));
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
	//add signal event here
		if(!SetEvent(Event))
		{
			_tprintf(_T("SetEvent failed(%d)\n"),GetLastError());
			for(i=0;i<THREADN;i++){
			CloseHandle(ThreadHandle[i]);
			CloseHandle(Event);
			}
			return -1;
		}
		_tprintf(_T("SetEvent successful\n"));
	//reset when last thread receives semaphore
		
		//if(counter1 == 4)
			//ResetEvent(Event);
		
	LeaveCriticalSection(&cs);
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
	
	}	
	_tprintf(_T("\nwait reply successfully\n"));
	//reset counter1
	EnterCriticalSection(&cs);
	counter1 = 0;
	LeaveCriticalSection(&cs);
	ResetEvent(Event_r);
	}
	_tprintf(_T("read finish!\n"));//other threads may be waiting for other events,
	//so needs to terminate other threads by main threads instead of terminate by themselves 
	Sleep(5000);
	
	//Set Event_f to be signaled,no need,just finish
	/*if(!SetEvent(Event_f))
	{
		_tprintf(_T("SetEvent failed (%d)\n"),GetLastError()); 
	}

	WaitForMultipleObjects(THREADN,ThreadHandle,TRUE,INFINITE);*/
	for(i=0;i<THREADN;i++){
		TerminateThread(ThreadHandle[i],NULL);
		CloseHandle(ThreadHandle[i]);
	}

	CloseHandle(Event);
	
	Sleep(10000000000000);
	return 0;
}

DWORD WINAPI sum(LPTSTR param)
{
	threads *th = (threads *)param;
	DWORD buf_sum = 0;
	DWORD i=0,j=0,k;

	//repeat till receive final osemaphore,so add check final semaphore here
	while(1){
	//wait for event
	WaitForSingleObject(Event,INFINITE);
	EnterCriticalSection(&cs);
	if(i<counter){
	counter1++;
	//i = _tcslen(buf);nono
	i = counter;
	while(j<i)
	{
		_tprintf(_T("buf[j]=%d "),buf[j]);
	//do the summation here
		buf_sum += buf[j];
		j++;
	}
	
	if(counter1 == THREADN){
		if(!SetEvent(Event_r))
		{
			_tprintf(_T("SetEvent Event_r[0] failed(%d)\n"),GetLastError());
			counter1 == 0;
			ResetEvent(Event);
			return -1;
		}
	}
	_tprintf(_T("result of sum is %d"),buf_sum);
	}
	LeaveCriticalSection(&cs);
	
	
	}
	_tprintf(_T("The final result of sum is %d\n"),buf_sum);
	return 0;
}

DWORD WINAPI product(LPTSTR param)
{
	threads *th = (threads *)param;
	DWORD buf_product = 1;
	DWORD i=0,j=0;
	DWORD dwResult1;

	//repeat till receive final semaphore,so add check final semaphore here
	while(1){
	
	dwResult1 = WaitForSingleObject(Event,INFINITE);
	if(dwResult1!=WAIT_OBJECT_0)
	{
		_tprintf(_T("%d didn't get event successfully,Error:%x\n"),th->thId,GetLastError());
		return 0;
	}
	EnterCriticalSection(&cs);
	//i = _tcslen(buf);nono
	if(i<counter){
	i = counter;
	while(j<i)
	{
		buf_product *= buf[j];
		j++;
	}
	_tprintf(_T("The result is of product is %d"),buf_product);
	//j=i;
	
	//add signal event here
	//counter_f++;
	if(counter1 ==THREADN){
		if(!SetEvent(Event_r))
		{
			_tprintf(_T("SetEvent Event_r failed(%d)\n"),GetLastError());
		}
	counter1 == 0;
	ResetEvent(Event);
	}
	_tprintf(_T("The result is of product is %d"),buf_product);
	}
	LeaveCriticalSection(&cs);
}
	_tprintf(_T("The final result is of product is %d"),buf_product);
	return 0;
}

DWORD WINAPI factorial(LPTSTR param)
{

	threads *th = (threads *)param;
	DWORD buf_factorial = 1;
	DWORD i=0,j,k;
	DWORD dwResult,dwResult1;

	//repeat till receive final event,so add check final semaphore here
	while(1){
	
	dwResult1 = WaitForSingleObject(Event,INFINITE);
	
	if(dwResult1!=WAIT_OBJECT_0)
	{
		_tprintf(_T("%d didn't get semaphore successfully.Error:%x\n"),th->thId,GetLastError());
		return 0;
	}
	
	//i = _tcslen(buf);nono
	EnterCriticalSection(&cs);
	if(i<counter){
	counter1++;
	for( k=i;k<counter;k++ ){
	if(buf[k]==0)
		_tprintf(_T("The result of factorial for %d is %d\n"),buf[k],0);
	else{
	for(j=1;j<=buf[k];j++)
	{
		buf_factorial *= j;
	}
	_tprintf(_T("The  result of factorial for %d is %d\n"),buf[k],buf_factorial);
	}
	//counter_f++;
	if(counter1 == THREADN){
	if(!SetEvent(Event_r))
		{
			_tprintf(_T("SetEvent Event_r failed(%d)\n"),GetLastError());
		}
	counter1 = 0;
	ResetEvent(Event);
	}
	buf_factorial = 1;
}
	//reset to 1
	i = counter;
}
	LeaveCriticalSection(&cs);
	//j=i;
	
	}
	_tprintf(_T("factorial finish!\n"));
	return 0;
}

DWORD WINAPI character(LPTSTR param){
	threads *th = (threads *)param;
	DWORD buf_factorial = 1;
	DWORD i=0,j,k;
	DWORD dwResult,dwResult1;

	//repeat till receive final event,so add check final semaphore here
	while(1){
	
	WaitForSingleObject(Event,INFINITE);
	EnterCriticalSection(&cs);
	counter1++;
	//i = _tcslen(buf);nono
	for(j=i;j<counter;j++){
		_tprintf(_T("%d:"),buf[j]);
		for(i=0;i<buf[j];i++)
			_tprintf(_T("#"));
		_tprintf(_T("\n"));
	}
	i = counter;
	if( counter1 ==THREADN ){
	if(!SetEvent(Event_r))
		{
			_tprintf(_T("SetEvent Event_r[1] failed(%d)\n"),GetLastError());
		}
	counter1 = 0;
	ResetEvent(Event);
	}
	LeaveCriticalSection(&cs);
	}
	_tprintf(_T("finish for '#'\n"));
	return 0;
}

int createFile(LPTSTR file){
	_tprintf(_T("Please input integer numbers(separated with space and terminated with '-1')"));
	INT i=0,j;
	HANDLE hIn,hOut;
	DWORD nWrt,nRd;

//scanf always expect \n,so enter after every number
_tscanf(_T("%d"),&buf[i]);//don't forget &!!!!!!
_tprintf(_T("%d "),buf[i]);
++i;
	while(buf[i-1]!=-1)//\r\n in windows
	{
		_tscanf(_T("%d"),&buf[i]);//don't forget &!!!!!!
		_tprintf(_T("%d "),buf[i]);
		++i;
	}
	i--;//to avoid -1 to be stored

	_tprintf(_T("input finishes!\n"));
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
	//don't forget clear when u use same buffer
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
	_tprintf(_T("check file:\n"));
	for(j=0;j<i;j++)
	{
		_tprintf(_T("%d "),buf[j]);
	}
	CloseHandle(hOut);
	return 0;
}