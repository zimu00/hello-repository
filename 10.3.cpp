                
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

#define MAX_SEM_COUNT 10

HANDLE mutex;
HANDLE Semaphore;

typedef struct Threads
{
	DWORD thId;
	TCHAR name[NAME];
	DWORD level;
}Threads_t;

typedef struct Threads_cc
{
	DWORD thId;
	DWORD nCount;
	Threads_t th[MAX_SEM_COUNT];
	boolean equal;
}Threads_c;

DWORD WINAPI Compare(LPVOID);
DWORD WINAPI TraverseDirectoryRecursive(LPVOID);
DWORD FileType (LPWIN32_FIND_DATA pFileData);

int _tmain (int argc, LPTSTR argv [])
{
	HANDLE *ThreadHandle;
	int i=0;
	Threads_t *thread;
	Threads_c thread_c;

	thread_c.equal = 0;
	ThreadHandle = (HANDLE *)malloc(sizeof(HANDLE)*(argc-1));
	thread = (Threads_t *)malloc(sizeof(Threads_t)*(argc-1));

	/// Create a semaphore with initial and max counts of MAX_SEM_COUNT/Create a mutex with no initial owner
	Semaphore = CreateSemaphore(
			NULL, // default security attributes
			0,// initially not owned
			MAX_SEM_COUNT,//maximum count
			NULL// unnamed semaphore
		);
	if(Semaphore == NULL)
	{
		_tprintf(_T("CreateSemaphore error:%d\n"),GetLastError());
		Sleep(1000000);
		return 1;
	}

	for(i=1;i<argc;i++){
		_tprintf(_T("argv[%d]=%s "),i,argv[i]);
	}

	thread_c.nCount = argc-1;

	for(i=0;i<argc-1;i++){
		//thread[i].name=NULL;
		memset(thread[i].name,0,NAME);
		_tprintf(_T("\n-->Visit Dir %s\n"),argv[i]);
		//_stprintf(thread[i].name,_T("%s"),thread[i].name);
		_tcscpy(thread[i].name,argv[i]);
		thread[i].level = 1;
		
		ThreadHandle[i] = CreateThread(
			0,0,(LPTHREAD_START_ROUTINE)TraverseDirectoryRecursive,(PVOID)&thread[i],0,&thread[i].thId);
		thread_c.th[i] = thread[i];
	}
	ThreadHandle[i] = CreateThread(
		0,0,(LPTHREAD_START_ROUTINE)Compare,(PVOID)&thread_c,0,&thread_c.thId);


	DWORD re = WaitForMultipleObjects(argc-1,ThreadHandle,TRUE,INFINITE);
	if(re>=WAIT_OBJECT_0 && re<=(WAIT_OBJECT_0+argc-2))
	_tprintf(_T("all %d threads read finish!\n"),argc-1);
	else {
		_tprintf(_T("error in waiting %d threads!\n"),argc-1);
	}

	Sleep(10000000);
	return 0;
}

DWORD WINAPI  TraverseDirectoryRecursive (LPVOID para)//use of level?
{
	Threads_t *th = (Threads_t *)para;
	HANDLE SearchHandle;
	WIN32_FIND_DATA FindData;
	DWORD FType, i;
	DWORD dwWaitResult;


	SetCurrentDirectory(th->name);

	SearchHandle = FindFirstFile(_T("*"),&FindData);

	do {
		FType = FileType(&FindData);

		if(FType == TYPE_FILE){
			_tcscpy(th->name,FindData.cFileName);
			for(i=0;i<th->level;i++)
				_tprintf(_T("  "));
			_tprintf(_T("%d level=%d FILE:%s\n"),th->thId,th->level,FindData.cFileName);
			if (!ReleaseSemaphore( 
                        Semaphore,  // handle to semaphore
                        1,            // increase count by one
                        NULL) )       // not interested in previous count
                {
                    printf("ReleaseSemaphore error: %d\n", GetLastError());
                }
			/*dwWaitResult = WaitForSingleObject(
				Semaphore,
				INFINITE
				);*/
		}
		if(FType == TYPE_DIR){
			//int i=CopyFile(FindData.cFileName,fullNewPath,FALSE);

			for(i=0;i<th->level;i++)
				_tprintf(_T("  "));
			_tprintf(_T("%d level=%d DIR: %s\n"),th->thId,th->level,FindData.cFileName);
			
			if (!ReleaseSemaphore( 
                        Semaphore,  // handle to semaphore
                        1,            // increase count by one
                        NULL) )       // not interested in previous count
                {
                    printf("ReleaseSemaphore error: %d\n", GetLastError());
                }
			/*dwWaitResult = WaitForSingleObject(
				Semaphore,
				INFINITE
				);*/

			th->level++;
			_tcscpy(th->name,FindData.cFileName);
			TraverseDirectoryRecursive(th);
			//SetCurrentDirectory(fullNewPath);
			
			SetCurrentDirectory(_T(".."));
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

DWORD WINAPI Compare(LPVOID para){
	
	//DWORD dwRet;
	Threads_c *thread = (Threads *)para;
	INT i;
	TCHAR name[NAME];

	WaitForMultipleObjects(thread->nCount,&Semaphore,FALSE,INFINITE);
	//compare starts....
	_tcscpy(name,(thread->th[0]).name);
	for(i=1;i<thread->nCount;i++)
	{
		if(_tcscmp((thread->th[i]).name,name)!=0){
			thread->equal = 1;
			break;
		}
	}

}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            