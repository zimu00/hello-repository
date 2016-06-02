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

HANDLE mutex;

/*struct Data{
	TCHAR name[NAME];
	DWORD size;
};*/

typedef struct Threads
{
	DWORD thId;
	//LPTSTR name;
	TCHAR name[NAME];
	DWORD level;
	TCHAR path[L];
}Threads_t;

DWORD WINAPI TraverseDirectoryRecursive(LPVOID);
DWORD FileType (LPWIN32_FIND_DATA pFileData);

int _tmain (int argc, LPTSTR argv [])
{
	HANDLE *ThreadHandle;
	int i=0;
	Threads_t *thread;

	ThreadHandle = (HANDLE *)malloc(sizeof(HANDLE)*(argc-1));
	thread = (Threads_t *)malloc(sizeof(Threads_t)*(argc-1));

	//Create a mutex with no initial owner
	mutex = CreateMutex(
			NULL, // default security attributes
			FALSE,// initially not owned
			NULL// unnamed mutex
		);

	for(i=1;i<argc;i++){
		_tprintf(_T("argv[%d]=%s "),i,argv[i]);
	}

	for(i=0;i<argc-1;i++){
		//thread[i].name=NULL;
		memset(thread[i].name,0,NAME);
		_tprintf(_T("\n-->Visit Dir %s\n"),argv[i]);
		//_stprintf(thread[i].name,_T("%s"),thread[i].name);
		_tcscpy(thread[i].path ,argv[i]);
		thread[i].level = 1;
		ThreadHandle[i] = CreateThread(
			0,0,(LPTHREAD_START_ROUTINE)TraverseDirectoryRecursive,(PVOID)&thread[i],0,&thread[i].thId);
	}

	DWORD re = WaitForMultipleObjects(argc-1,ThreadHandle,TRUE,INFINITE);
	if(re>=WAIT_OBJECT_0 && re<=(WAIT_OBJECT_0+argc-2))
	_tprintf(_T("all %d threads read finish!\n"),argc-1);
	else {
		_tprintf(_T("error in waiting %d threads!\n"),argc-1);
	}

	Sleep(10000000);
	return 0;
}

DWORD WINAPI  TraverseDirectoryRecursive (LPVOID para)
{
	Threads_t *th = (Threads_t *)para;
	HANDLE SearchHandle;
	WIN32_FIND_DATA FindData;
	TCHAR SearchPath[L];
	TCHAR save_path[L];
	DWORD FType, i;


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
			//request ownership
		WaitForSingleObject(mutex,INFINITE);
		_tprintf(_T("\n%d has got the mutex\n"),th->thId);

			for(i=0;i<th->level;i++)
				_tprintf(_T("  "));
			_tprintf(_T("%d level=%d FILE:%s\n"),th->thId,th->level,FindData.cFileName);
		
//release ownership of the mutex object
if(!ReleaseMutex(mutex))
{
_ftprintf(stderr,_T("%d Release failed.Error:%x\n"),th->thId,GetLastError());
return 9;
}
else 
_tprintf(_T("%d released the mutex\n"),th->thId);

		}
		if(FType == TYPE_DIR){
			_tcscpy(th->name,FindData.cFileName);

			//request ownership
		WaitForSingleObject(mutex,INFINITE);
		_tprintf(_T("\n%d has got the mutex\n"),th->thId);

			_tprintf(_T("%d DIR: %s\n"),th->thId,FindData.cFileName);
			//int i=CopyFile(FindData.cFileName,fullNewPath,FALSE);

			for(i=0;i<th->level;i++)
				_tprintf(_T("  "));
			_tprintf(_T("%d level=%d DIR: %s\n"),th->thId,th->level,FindData.cFileName);
			
			//release ownership of the mutex object
if(!ReleaseMutex(mutex))
{
_ftprintf(stderr,_T("%d Release failed.Error:%x\n"),th->thId,GetLastError());
return 9;
}
else 
_tprintf(_T("%d released the mutex\n"),th->thId);


			th->level++;
			_tcscpy(th->name,FindData.cFileName);
			TraverseDirectoryRecursive(th);
			//SetCurrentDirectory(fullNewPath);
			
			//---------to return to upper layer---------
			_tprintf(_T("return to previous layer\n"));
			_tcscpy(th->path,save_path);
			_tprintf(_T("previous path is %s\n"),th->path);
		}
		
	} while (FindNextFile(SearchHandle,&FindData));
	
	FindClose(SearchHandle);

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
