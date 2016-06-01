#define _CRT_NON_CONFORMING_SWPRINTFS
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#define L 50000
#define NAME  100

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_DOT 3


typedef struct Threads
{
	DWORD thId;
	TCHAR name[NAME];
	TCHAR path[L];
	TCHAR fileName[NAME];
}Threads_t;

DWORD WINAPI  PreCreatFile (LPVOID para);
DWORD TraverseDirectoryRecursive(Threads_t *,HANDLE);
DWORD FileType (LPWIN32_FIND_DATA pFileData);

int _tmain (int argc, LPTSTR argv [])
{
	HANDLE *ThreadHandle,h;
	int i=0;
	DWORD n=0;
	Threads_t **thread;
	Threads_t th_check;
	TCHAR fileName[L];

	_stprintf(fileName,_T("%s"),fileName);

	ThreadHandle = (HANDLE *)malloc(sizeof(HANDLE)*(argc-1));
	thread = (Threads_t **)malloc(sizeof(Threads_t)*(argc-1));
	for(;i<argc-1;i++)
		thread[i]=(Threads_t *)malloc(sizeof(Threads_t));

	for(i=1;i<argc;i++){
		_tprintf(_T("argv[%d]=%s "),i,argv[i]);
	}

	for(i=0;i<argc-1;i++){
		//thread[i].name=NULL;
		memset(thread[i]->name,0,NAME);
		_tprintf(_T("\n-->Visit Dir %s\n"),argv[i+1]);
		_stprintf(thread[i]->name,_T("%s"),thread[i]->name);
		_tcscpy(thread[i]->path,argv[i+1]);
		
		ThreadHandle[i] = CreateThread(
			0,0,(LPTHREAD_START_ROUTINE)PreCreatFile,(PVOID)thread[i],0,&thread[i]->thId);
	}
	WaitForMultipleObjects(argc-1,ThreadHandle,TRUE,INFINITE);
	
	TCHAR path[L];
	for(i=0;i<argc-1;i++){
		//SetCurrentDirectory(thread[i].filePath);

		_tprintf(_T("filename is %s\n"),thread[i]->name);
		//GetFullPathName(thread[i].fileName,L,path,NULL);
		//_tprintf(_T("CurrentDirectory is %s\n"),path);
		h = CreateFile(
			thread[i]->fileName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if( h == INVALID_HANDLE_VALUE ){
			_ftprintf(stderr,_T("Cannot open output file %s.Error:%x\n"),thread[i]->fileName,GetLastError());
		CloseHandle(h);
		Sleep(2000000);
		return 3;
	}
		_tprintf(_T("check file\n"));
		while (ReadFile(h,&th_check,sizeof(Threads_t),&n,NULL) && n>0 ){
		if( n==sizeof(Threads_t) )
			_tprintf(_T("%d %s\n"),th_check.thId,th_check.name);
		else{
			_ftprintf(stderr,_T("Didn't read successfully %s.Error:%x\n"),thread[i]->name,GetLastError());
			CloseHandle(h);
			Sleep(2000000);
			return 3;
		}
		}
		CloseHandle(h);
	}
		

	Sleep(10000000);
	return 0;
}

DWORD WINAPI  PreCreatFile (LPVOID para){
	Threads_t *th = (Threads_t *)para;
	HANDLE hOut;
	TCHAR temp[L];
	DWORD level;

	memset(temp,0,L);

	_itot(th->thId,temp,10);
	_tcscpy(th->fileName,temp);
	_tprintf(_T("----------fileName is %s\n"),th->fileName);

	hOut = CreateFile(temp,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hOut == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot open output file %d.Error:%x\n"),temp,GetLastError());
		CloseHandle(hOut);
		return 4;
	}

	CloseHandle(hOut);
	//GetFullPathName(th->fileName,L,th->fileName,NULL);
	_tprintf(_T("\n-----filename is %s\n"),temp);

	hOut = CreateFile(temp,GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hOut == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("%d Cannot open output file %s.Error:%x\n"),th->thId,th->name,GetLastError());
		CloseHandle(hOut);
		return 4;
	}

	TraverseDirectoryRecursive(th,hOut);
		CloseHandle(hOut);
	return 0;
}

DWORD TraverseDirectoryRecursive (Threads_t *th,HANDLE hOut)
{  
	HANDLE SearchHandle;
	WIN32_FIND_DATA FindData;
	DWORD FType;
	DWORD dwWaitResult;
	TCHAR SearchPath[L];
	TCHAR save_path[L];
	DWORD nWrt;
	
	_stprintf(SearchPath,_T("%s\\*"),th->path);
	_tprintf(_T("%d SearchPath is %s\n"),th->thId,SearchPath);
	_tcscpy(save_path,th->path);
	SearchHandle = FindFirstFile(SearchPath,&FindData);
	if(SearchHandle==INVALID_HANDLE_VALUE)
	{
		_ftprintf(stderr,_T("%d FindFirstFile.Error:%x\n"),th->thId,GetLastError());
		return 9;
	}

	do {
		FType = FileType(&FindData);

		if(FType == TYPE_FILE){
			_tcscpy(th->name,FindData.cFileName);
			_tprintf(_T("%d FILE:%s\n"),th->thId,th->name);
			WriteFile(hOut,th,sizeof(Threads_t),&nWrt,NULL);
			if ( nWrt!=sizeof(Threads_t) ){
				_tprintf(_T("Error in writing file %x\n"),GetLastError());
				//CloseHandle(hOut);
				return 4;
			}
		
		}
		if(FType == TYPE_DIR){
			_tcscpy(th->name,FindData.cFileName);
			_tprintf(_T("%d DIR: %s\n"),th->thId,FindData.cFileName);
			WriteFile(hOut,th,sizeof(Threads_t),&nWrt,NULL);
			if ( nWrt!=sizeof(Threads_t) ){
				_tprintf(_T("Error in writing file %x\n"),GetLastError());
				//CloseHandle(hOut);
				return 4;
			}
			
			_stprintf(th->path,_T("%s\\%s"),th->path,FindData.cFileName);
			_tprintf(_T("the new path is %s\n"),th->path);
			TraverseDirectoryRecursive(th,hOut);
	
			//---------to return to upper layer---------
			_tprintf(_T("return to previous layer\n"));
			_tcscpy(th->path,save_path);
			_tprintf(_T("previous path is %s\n"),th->path);
		}
		
	} while (FindNextFile(SearchHandle,&FindData));
	
	FindClose(SearchHandle);
	_tprintf(_T("%d finishes\n"),th->thId);
	return 0;
}



DWORD FileType (LPWIN32_FIND_DATA pFileData)
{
	BOOL IsDir;
	DWORD FType;
	FType = TYPE_FILE;
	IsDir = (pFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	if(IsDir)
		if(lstrcmp(pFileData->cFileName,_T(".")) == 0
			|| lstrcmp(pFileData->cFileName,_T(".."))==0)
			FType = TYPE_DOT;
		else FType = TYPE_DIR;
		return FType;
}
