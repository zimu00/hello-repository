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
HANDLE semaphore[BUFLEN];
HANDLE fse;
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
	int i,k;
	HANDLE h;
	DWORD n,temp;

	buf[0]='\0';
	createFile(argv[1]);
	ThreadHandle = (HANDLE *)malloc(sizeof(HANDLE)*(THREADN));
	th = (threads *)malloc(sizeof(HANDLE)*(THREADN));
	memset(buf,0,sizeof(BUFLEN));
	for(k=0;k<THREADN;k++)
	semaphore[k] = CreateSemaphore(NULL,0,MAX_SEM_NUM,NULL);
	fse = CreateSemaphore(NULL,0,THREADN,NULL);
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
	//add sending semaphore here
	for(k=0;k<THREADN;k++){
	if (!ReleaseSemaphore( 
                        semaphore[k],  // handle to semaphore
						1,            // increase count by four
                        NULL) )       //interested in previous count
                {
					printf(" ReleaseSemaphore error: %d\n", GetLastError());
                }
			else{
				_tprintf(_T(" have released semaphore\n"));
			}
	}
	}
	_tprintf(_T("read finish!\n"));
	Sleep(5000);
	//add sending final semaphore here
	if (!ReleaseSemaphore( 
                        fse,  // handle to semaphore
						THREADN,            // increase count by one
                        NULL) )       //interested in previous count
                {
					printf(" ReleaseSemaphore error: %d\n", GetLastError());
                }
			else{
				_tprintf(_T(" have released semaphore\n"));
			}

	WaitForMultipleObjects(THREADN,ThreadHandle,TRUE,INFINITE);
	for(i=0;i<THREADN;i++){
		CloseHandle(ThreadHandle[i]);
	}

	Sleep(10000000000000);
	return 0;
}

DWORD WINAPI sum(LPTSTR param)
{
	threads *th = (threads *)param;
	DWORD buf_sum = 0;
	DWORD i=0,j=0;
	DWORD dwResult,dwResult1;

	//repeat till receive final osemaphore,so add check final semaphore here
	while(1){
	dwResult = WaitForSingleObject(fse,TIMEOUT);
	if(dwResult!=WAIT_OBJECT_0  || (i<counter) ){
	//wait for semaphore
	dwResult1 = WaitForSingleObject(semaphore[0],INFINITE);
	if(dwResult1!=WAIT_OBJECT_0)
	{
		_tprintf(_T("%d didn't get semaphore successfully.Error:%x\n"),th->thId,GetLastError());
		return 0;
	}
	EnterCriticalSection(&cs);
	//i = _tcslen(buf);nono
	i = counter;
	while(j<i)
	{
		_tprintf(_T("buf[j]=%d "),buf[j]);
	//do the summation here
		buf_sum += buf[j];
		j++;
	}
	LeaveCriticalSection(&cs);
	/*if (!ReleaseSemaphore( 
                        semaphore,  // handle to semaphore
                        1,            // increase count by one
                        NULL) )       //interested in previous count
                {
					printf("%d ReleaseSemaphore error: %d\n",th->thId, GetLastError());
                }
			else{
				_tprintf(_T("%d have released semaphore\n"),th->thId);
			}*/
	//j=i;
	//_tprintf(_T("The result of sum is %d\n"),buf_sum);
	}
	break;
	}
	_tprintf(_T("The final result of sum is %d\n"),buf_sum);
	return 0;
}
DWORD WINAPI product(LPTSTR param)
{
	threads *th = (threads *)param;
	DWORD buf_product = 1;
	DWORD i=0,j=0;
	DWORD dwResult,dwResult1;

	//repeat till receive final semaphore,so add check final semaphore here
	while(1){
	dwResult = WaitForSingleObject(fse,TIMEOUT);
	if((dwResult!=WAIT_OBJECT_0)  || (i==counter) ){
	//wait for semaphore
	dwResult1 = WaitForSingleObject(semaphore[1],INFINITE);
	if(dwResult1!=WAIT_OBJECT_0)
	{
		_tprintf(_T("%d didn't get semaphore successfully,Error:%x\n"),th->thId,GetLastError());
		return 0;
	}
	EnterCriticalSection(&cs);
	//i = _tcslen(buf);nono
	i = counter;
	while(j<i)
	{
		buf_product *= buf[j];
		j++;
	}
	LeaveCriticalSection(&cs);
	//j=i;
	_tprintf(_T("The result is of product is %d"),buf_product);
	}
	break;
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

	//repeat till receive final semaphore,so add check final semaphore here
	//while(){
	while(1){
	dwResult = WaitForSingleObject(fse,TIMEOUT);

	if((dwResult!=WAIT_OBJECT_0)  || (i<counter) ){
	//wait for semaphore
	dwResult1 = WaitForSingleObject(semaphore[2],INFINITE);
	if(dwResult1!=WAIT_OBJECT_0)
	{
		_tprintf(_T("%d didn't get semaphore successfully.Error:%x\n"),th->thId,GetLastError());
		return 0;
	}
	EnterCriticalSection(&cs);
	//i = _tcslen(buf);nono

	for( k=i;k<counter;k++ ){
	if(buf[k]==0)
		_tprintf(_T("The final result of factorial for %d is %d\n"),buf[k],0);
	else{
	for(j=1;j<=buf[k];j++)
	{
		buf_factorial *= j;
	}
	_tprintf(_T("The final result of factorial for %d is %d\n"),buf[k],buf_factorial);
	}
	buf_factorial = 1;
	}
	//reset to 1
	LeaveCriticalSection(&cs);
	//j=i;
	i = counter;
	}
	break;
	_tprintf(_T("factorial finish!\n"));
	}
	return 0;
}
DWORD WINAPI character(LPTSTR param){
	threads *th = (threads *)param;
	DWORD buf_factorial = 1;
	DWORD i=0,j,k;
	DWORD dwResult,dwResult1;

	//repeat till receive final semaphore,so add check final semaphore here
	while(1){
	dwResult = WaitForSingleObject(fse,TIMEOUT);

	if((dwResult!=WAIT_OBJECT_0)  || (i<counter) ){
	dwResult1 = WaitForSingleObject(semaphore[3],INFINITE);
	if( dwResult1!=WAIT_OBJECT_0 )
	EnterCriticalSection(&cs);
	//i = _tcslen(buf);nono
	for(j=0;j<counter;j++){
		_tprintf(_T("%d:"),buf[j]);
		for(i=0;i<buf[j];i++)
			_tprintf(_T("#"));
		_tprintf(_T("\n"));
	}

	LeaveCriticalSection(&cs);
	//j=i;
	}
	break;
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