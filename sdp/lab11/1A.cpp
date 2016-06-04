#pragma warning(disable:4996)
#define _CRT_NON_CONFORMING_SWPRINTFS
#define UNICODE
#define _UNICODE

#define NAME 200

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Threads{

	DWORD thId;
	TCHAR name[NAME];
	HANDLE hOut;
}Threads_t;

typedef struct Records{
	INT row;
	DWORD id;
	TCHAR name[NAME];
	TCHAR surname[NAME];
	DWORD amount;
}Record;

DWORD WINAPI operation(LPVOID param);
TCHAR name_account[NAME];

int _tmain(int argc,LPTSTR argv []){
	HANDLE *ThreadHandle,h,hOut,hIn;
	Threads_t *thread;
	int i;
	DWORD n;
	Record record;
	
	ThreadHandle = (HANDLE *)malloc(sizeof(HANDLE)*(argc-1));
	thread = (Threads_t *)malloc(sizeof(HANDLE)*(argc-1));
	_tcscpy(name_account,argv[1]);
	_tprintf(_T("Account file name is %s\n"),name_account);

	_tprintf(_T("file before updating is:\n"));
	hIn=CreateFile(name_account,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hIn==INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot open output file.Error:%x\n"),GetLastError());
		CloseHandle(h);
		Sleep(10000000);
		return 5;
	}
	while(ReadFile(hIn,&record,sizeof(Record),&n,NULL)&&n>0){
	if(n!=sizeof(Record)){
		_ftprintf(stderr,_T("Read %d error:%x\n"), sizeof(Record),GetLastError());
		CloseHandle(h);
		Sleep(100000000);
		return 5;
	}
	else
		_tprintf(_T("%d %ld %s %s %ld\n"),record.row,record.id,record.name,record.surname,record.amount);
	}
	CloseHandle(hIn);

	hOut = CreateFile(name_account,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hOut == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot open input file.Error:%x\n"),GetLastError());
		CloseHandle(hOut);
		Sleep(200000);
		return 1;
	}
	
	//create threads
	for(i=0;i<argc-2;i++){
		_tcscpy(thread[i].name,argv[i+2]);
		thread[i].hOut = hOut;
		ThreadHandle[i] = CreateThread(0,0,(LPTHREAD_START_ROUTINE)operation,&thread[i],0,&thread[i].thId); 
	}

	//wait for finish of threads
	WaitForMultipleObjects(argc-2,ThreadHandle,TRUE,INFINITE);
	for(i=0;i<argc-2;i++)
		CloseHandle(ThreadHandle[i]);
	CloseHandle(hOut);

	_tprintf(_T("file before updating is:\n"));
	h=CreateFile(name_account,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(h==INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot open output file.Error:%x\n"),GetLastError());
		CloseHandle(h);
		Sleep(10000000);
		return 5;
	}
	while(ReadFile(h,&record,sizeof(Record),&n,NULL)&&n>0){
	if(n!=sizeof(Record)){
		_ftprintf(stderr,_T("Read %d error:%x\n"), sizeof(Record),GetLastError());
		CloseHandle(h);
		Sleep(100000000);
		return 5;
	}
	else
		_tprintf(_T("%d %ld %s %s %ld\n"),record.row,record.id,record.name,record.surname,record.amount);
	}
	free(ThreadHandle);
	//free(thread);
	Sleep(200000000);
	return 0;
}

DWORD WINAPI operation(LPVOID param){

	Threads_t *th= (Threads_t *)param;
	HANDLE hIn;
	DWORD nIn,nOut;
	Record record;
	LARGE_INTEGER fileReserved,filePos;
	OVERLAPPED ov = {0,0,0,0,NULL};

	_tprintf(_T("%d operation file name is %s\n"),th->thId,th->name);

	hIn = CreateFile(th->name,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hIn == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("%d Cannot open input file.Error:%x\n"),th->thId,GetLastError());
		CloseHandle(hIn);
		return 1;
	}

	while(ReadFile(hIn,&record,sizeof(Record),&nIn,NULL)&&nIn>0){
		if(nIn != sizeof(Record)){
			_ftprintf(stderr,_T("%d Fatal read %d nIn=%d,error: %x\n"),th->thId,sizeof(Record),nIn,GetLastError());
			CloseHandle(hIn);
			Sleep(200000);
			return 3;
		}
		else{
			fileReserved.QuadPart=1*sizeof(record);
			filePos.QuadPart = (record.row-1)*sizeof(record);
			ov.Offset = filePos.LowPart;
			ov.OffsetHigh = filePos.HighPart;
			ov.hEvent = 0;
			//lockfile
			LockFileEx(th->hOut,LOCKFILE_EXCLUSIVE_LOCK,0,fileReserved.LowPart,fileReserved.HighPart,&ov);
			
			WriteFile(th->hOut,&record,sizeof(record),&nOut,&ov);	
			UnlockFileEx(th->hOut,0,fileReserved.LowPart,fileReserved.HighPart,&ov);
			if(nOut!=nIn){
				_ftprintf(stderr,_T("%d fatal write %d error:%x\n"),th->thId,sizeof(Record),GetLastError());
				CloseHandle(hIn);
				Sleep(200000);
				return 4;
			}
		}
	}
	CloseHandle(hIn);
	return 0;
}

