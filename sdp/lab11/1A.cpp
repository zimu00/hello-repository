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

}Threads_t;

typedef struct Records{
	DWORD row;
	DWORD id;
	TCHAR name[NAME];
	TCHAR surname[NAME];
	DWORD amount;
}Record;

DWORD WINAPI operation(LPVOID param);
TCHAR name_account[NAME];

int _tmain(int argc,LPTSTR argv []){
	HANDLE *ThreadHandle;
	Threads_t *thread;
	int i;
	
	ThreadHandle = (HANDLE *)malloc(sizeof(HANDLE)*(argc-1));
	thread = (Threads_t *)malloc(sizeof(HANDLE)*(argc-1));
	_tcscpy(name_account,argv[1]);
	_tprintf(_T("Account file name is %s\n"),name_account);

	//create threads
	for(i=0;i<argc-2;i++){
		_tcscpy(thread[i].name,argv[i+2]);

		ThreadHandle[i] = CreateThread(0,0,(LPTHREAD_START_ROUTINE)operation,&thread[i],0,&thread[i].thId); 
	}

	//wait for finish of threads
	WaitForMultipleObjects(argc-2,ThreadHandle,TRUE,INFINITE);
	for(i=0;i<argc-2;i++)
		CloseHandle(ThreadHandle[i]);

	free(ThreadHandle);
	//free(thread);
	Sleep(200000000);
	return 0;
}

DWORD WINAPI operation(LPVOID param){

	Threads_t *th= (Threads_t *)param;
	HANDLE hIn,hOut,hIn2;
	DWORD nIn,nOut,nIn2;
	Record record,record2;
	LARGE_INTEGER fileReserved,filePos;
	OVERLAPPED ov = {0,0,0,0,NULL};

	_tprintf(_T("%d operation file name is %s\n"),th->thId,th->name);

	hIn = CreateFile(th->name,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hIn == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot open input file.Error:%x\n"),GetLastError());
		CloseHandle(hIn);
		return 1;
	}

	hIn2 = CreateFile(name_account,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hOut == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot open input file.Error:%x\n"),GetLastError());
		CloseHandle(hIn2);
		CloseHandle(hIn);
		return 1;
	}

	hOut = CreateFile(name_account,GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hOut == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot open input file.Error:%x\n"),GetLastError());
		CloseHandle(hIn);
		CloseHandle(hOut);
		CloseHandle(hIn2);
		Sleep(200000);
		return 1;
	}
	while(ReadFile(hIn,&record,sizeof(record),&nIn,NULL)&&nIn>0){
		if(nIn != sizeof(record)){
			_ftprintf(stderr,_T("Fatal read error: %x\n"),GetLastError());
			CloseHandle(hIn);
			CloseHandle(hOut);
			CloseHandle(hIn2);
			Sleep(200000);
			return 3;
		}
		else{
			fileReserved.QuadPart=1*sizeof(record);
			filePos.QuadPart = record.row*sizeof(record);
			ov.Offset = filePos.LowPart;
			ov.OffsetHigh = filePos.HighPart;
			ov.hEvent = 0;
			//lockfile
			LockFileEx(hOut,LOCKFILE_EXCLUSIVE_LOCK,0,fileReserved.LowPart,fileReserved.HighPart,&ov);
			
			ReadFile(hIn2,&record2,sizeof(Record),&nIn2,&ov);
			if(nIn != sizeof(record)){
			_ftprintf(stderr,_T("Fatal read error: %x\n"),GetLastError());
			CloseHandle(hIn);
			CloseHandle(hOut);
			CloseHandle(hIn2);
			Sleep(200000);
			return 3;
		}
			
			WriteFile(hOut,&record,sizeof(record),&nOut,&ov);	
			UnlockFileEx(hOut,0,fileReserved.LowPart,fileReserved.HighPart,&ov);
			if(nOut!=nIn){
				_ftprintf(stderr,_T("fatal write error:%x\n"),GetLastError());
				CloseHandle(hIn);
				CloseHandle(hOut);
				CloseHandle(hIn2);
				Sleep(200000);
				return 4;
			}
		}
	}
	CloseHandle(hIn);
	CloseHandle(hOut);
	return 0;
}
