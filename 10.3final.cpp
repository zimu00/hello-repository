#pragma warning(disable:4996)
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

HANDLE Semaphore;
HANDLE cSemaphore;
HANDLE fSemaphore;

typedef struct Threads
{
	DWORD thId;
	TCHAR name[NAME];
	TCHAR path[NAME];
	DWORD equal;
	DWORD id;
}Threads_t;

typedef struct Threads_cc
{
	DWORD thId;
	
	DWORD nCount;
	//Threads_t th[MAX_SEM_COUNT];
	HANDLE threadh[MAX_SEM_COUNT];
	Threads_t *th;
	boolean equal;
}Threads_c;

DWORD WINAPI Compare(LPVOID);
DWORD WINAPI TraverseDirectoryRecursive(LPVOID);
DWORD FileType (LPWIN32_FIND_DATA pFileData);
Threads_c thread_c;

int _tmain (int argc, LPTSTR argv [])
{
	HANDLE *ThreadHandle;
	int i=0;
	Threads_t **thread;
	

	
	ThreadHandle = (HANDLE *)malloc(sizeof(HANDLE)*(argc-1));
	thread = (Threads_t **)malloc(sizeof(Threads_t)*(argc-1));
	for(;i<argc-1;i++){
		thread[i] = (Threads_t *)malloc(sizeof(Threads_t));
	}
	thread_c.th = (Threads_t *)malloc(sizeof(Threads_t)*(argc-1));
	

	thread_c.equal = 0;
	/// Create a semaphore with initial and max counts of MAX_SEM_COUNT/Create a mutex with no initial owner
	Semaphore = CreateSemaphore(
			NULL, // default security attributes
			0,// initially not owned
			MAX_SEM_COUNT,//maximum count
			NULL// unnamed semaphore
		);
	cSemaphore = CreateSemaphore(
			NULL, // default security attributes
			0,// initially not owned
			MAX_SEM_COUNT,//maximum count
			NULL// unnamed semaphore
		);
	if(cSemaphore==NULL){
		_tprintf(_T("Create cSemaphore failed.error:%x\n"),GetLastError());
		return 1;
	}
	//for main_thread informs the completement of other threads to the comparing thread
	fSemaphore = CreateSemaphore(
			NULL, // default security attributes
			0,// initially not owned
			1,//maximum count
			NULL// unnamed semaphore
		);
	if(fSemaphore==NULL){
		_tprintf(_T("Create cSemaphore failed.error:%x\n"),GetLastError());
		return 1;
	}
	

	for(i=1;i<argc;i++){
		_tprintf(_T("argv[%d]=%s "),i,argv[i]);
	}

	thread_c.nCount = argc-1;

	for(i=0;i<argc-1;i++){
		//thread[i].name=NULL;
		memset(thread[i]->name,0,NAME);
		_tprintf(_T("\n-->Visit Dir %s\n"),argv[i+1]);
		//_stprintf(thread[i].name,_T("%s"),thread[i].name);
		_tcscpy(thread[i]->path,argv[i+1]);
		thread[i]->id = i;
		
		ThreadHandle[i] = CreateThread(
			0,0,(LPTHREAD_START_ROUTINE)TraverseDirectoryRecursive,(PVOID)thread[i],0,&thread[i]->thId);
		//thread_c.th[i] = *thread[i];????
		thread_c.threadh[i]=ThreadHandle[i];
	}
	//ThreadHandle[i] = CreateThread(
		//0,0,(LPTHREAD_START_ROUTINE)Compare,(PVOID)&thread_c,0,&thread_c.thId);
	ThreadHandle[i] = CreateThread(
		0,0,(LPTHREAD_START_ROUTINE)Compare,NULL,0,&thread_c.thId);

	//don't use waitForMultiple cz maybe 1st thread read a shorter directory tree,then finish
	//but others go on releasing semaphores and waiting
	/*DWORD re = WaitForMultipleObjects(argc-1,ThreadHandle,TRUE,INFINITE);
	if(re>=WAIT_OBJECT_0 && re<=(WAIT_OBJECT_0+argc-2))
	_tprintf(_T("all %d threads read finish!\n"),argc-1);
	else {
		_tprintf(_T("error in waiting %d threads!\n"),argc-1);
	}*/
	//check the termination of thread
	DWORD re;
	for(i=0;i<argc-1;i++){
		re = WaitForSingleObject(
			ThreadHandle[i],
			0
			);
		if(re == WAIT_OBJECT_0)
		{
			_tprintf(_T("Thread %d finished!\n"),thread_c.th[i].thId);
			break;
		}
		else{
			if(i=argc-1)
				i=0;
		}
	}

	if(thread_c.equal==1)
	{
		_tprintf(_T("\n---not all threads are equal,all exit!!!----\n"));
	}

	//inform the comparison thread
	/*if(!ReleaseSemaphore(
			fSemaphore,
			1,
			NULL
		))
	{
		_tprintf(_T("Main thread ReleaseSemaphore to comparison thread error:%d\n"),GetLastError());
	}
	else
	{
		_tprintf(_T("Main thread has release semaphore to comparison thread ,waiting now\n"));
	}
	WaitForSingleObject(ThreadHandle[i],INFINITE);
	//check the result
	if(thread_c.equal==1)
		_tprintf(_T("not all directories have same content\n"));
	else
		_tprintf(_T("all threads have same content\n"));
	*/
	CloseHandle(fSemaphore);
	CloseHandle(cSemaphore);
	CloseHandle(Semaphore);
	for(i=0;i<argc;i++)
		CloseHandle(ThreadHandle[i]);
	_tprintf(_T("all threads finished!\n"));
	Sleep(10000000);
	return 0;
}

DWORD WINAPI  TraverseDirectoryRecursive (LPVOID para)
{   LPTSTR par1=NULL;
	Threads_t *th = (Threads_t *)para;
	HANDLE SearchHandle;
	WIN32_FIND_DATA FindData;
	DWORD FType;
	DWORD dwWaitResult;
	TCHAR SearchPath[L];
	
	par1=SearchPath;
	_stprintf(SearchPath,_T("%s\\*"),th->path);
	_tprintf(_T("SearchPath is %s\n"),SearchPath);
	par1=SearchPath;
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
			thread_c.th[th->id] = *th;
		if (!ReleaseSemaphore( 
                        Semaphore,  // handle to semaphore
                        1,            // increase count by one
                        NULL) )       //interested in previous count
                {
					printf("%d ReleaseSemaphore error: %d\n",th->thId, GetLastError());
                }
			else{
				_tprintf(_T("%d have released semaphore\n"),th->thId);
			}
			dwWaitResult = WaitForSingleObject(
				cSemaphore,
				INFINITE
				);
			if(dwWaitResult==WAIT_OBJECT_0)
				_tprintf(_T("%d has got semaphore successfully\n"),th->thId);
			else
				_tprintf(_T("%d didn't get semaphore successfully\n"),th->thId);
			/*if(th->equal==1){
				_tprintf(_T("not equal,so %d exit\n"),th->thId);
				ExitThread(NULL);
				return 2;
			}*/
		
		}
		if(FType == TYPE_DIR){
			//int i=CopyFile(FindData.cFileName,fullNewPath,FALSE);
			_tcscpy(th->name,FindData.cFileName);
			_tprintf(_T("%d DIR: %s\n"),th->thId,FindData.cFileName);
			thread_c.th[th->id] = *th;
			if (!ReleaseSemaphore( 
                        Semaphore,  // handle to semaphore
                        1,            // increase count by one
                        NULL) )       //interested in previous count
                {
					printf("%d ReleaseSemaphore error: %d\n",th->thId, GetLastError());
                }
			else{
				_tprintf(_T("%d have released semaphore\n"),th->thId);
			}
			dwWaitResult = WaitForSingleObject(
				cSemaphore,
				INFINITE
				);
			if(dwWaitResult==WAIT_OBJECT_0)
				_tprintf(_T("%d has got semaphore successfully\n"),th->thId);
			else
				_tprintf(_T("%d didn't get semaphore successfully\n"),th->thId);
			/*if(th->equal==1){
				_tprintf(_T("not equal,so %d exit\n"),th->thId);
				ExitThread(NULL);
				return 2;
			}*/

			_stprintf(th->path,_T("%s\\%s"),th->path,FindData.cFileName);
			_tprintf(_T("the new path is %s\n"),th->path);
			TraverseDirectoryRecursive(th);
			//SetCurrentDirectory(fullNewPath);
			//SetCurrentDirectory(current_dire);//???
			//how to go upper layer
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
	IsDir = (pFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;//????

	if(IsDir)
		if(lstrcmp(pFileData->cFileName,_T(".")) == 0
			|| lstrcmp(pFileData->cFileName,_T(".."))==0)
			FType = TYPE_DOT;
		else FType = TYPE_DIR;
	/*if(pFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		FType = TYPE_DIR;
		return FType;
	}
	else*/
		return FType;
}

DWORD WINAPI Compare(LPVOID para){
	
	DWORD dwRet;
	//Threads_c *thread = (Threads_c *)para;
	DWORD i;
	DWORD j;
	TCHAR name[NAME];

	//don't need to compare root 
	/*for (i=0;i<thread->nCount;i++)
		dwRet = WaitForSingleObject(Semaphore,INFINITE);
	if (!ReleaseSemaphore(
		cSemaphore,
		thread->nCount,
		NULL
		))
	{
		_tprintf(_T("----->Comparison thread ReleaseSemaphore error:%d\n"),GetLastError());
	}
	else
	{
		_tprintf(_T("----->Comparison thread have release semaphore\n"));
	}*/
	//dwRet = WaitForMultipleObjects(thread->nCount,&Semaphore,FALSE,INFINITE);
	while(1)
	{
		/*dwRet = WaitForSingleObject(
			fSemaphore,
			0L //zeros timeout interval????
			);
		if(dwRet==WAIT_OBJECT_0)
		{
			_tprintf(_T("----->Comparison thread finishes\n"));
			return 0;
		}*/

	//WaitForMultipleObjects(thread->nCount,&Semaphore,FALSE,INFINITE);
		for (j=0;j<thread_c.nCount;j++)
		dwRet = WaitForSingleObject(Semaphore,INFINITE);
	//compare starts....
	_tcscpy(name,(thread_c.th[0]).name);
	_tprintf(_T("*****To check if every thread visits %s"),(thread_c.th[0]).name);
	for(i=1;i<thread_c.nCount;i++)
	{
		if( _tcscmp((thread_c.th[i]).name,name)!=0  ){
			thread_c.equal = 1;
			_tprintf(_T("thread %d visits %s,so not same"),(thread_c.th[i]).thId,(thread_c.th[i]).name);
			break;
		}
	}
	if(thread_c.equal!=0)
	{
		//terminate other threads
		for(i=0;i<thread_c.nCount;i++)
			TerminateThread(thread_c.threadh[i],NULL);

		//	(thread_c.th[i]).equal = 1;
		_tprintf(_T("\nnot all contents are equal\n"));
	}
	else
	{
		for(i=0;i<thread_c.nCount;i++)
			(thread_c.th[i]).equal = 0;
		_tprintf(_T("\nall contents are equal\n"));
	}

	if (!ReleaseSemaphore(
		cSemaphore,
		thread_c.nCount,
		NULL
		))
	{
		_tprintf(_T("----->Comparison thread ReleaseSemaphore error:%d\n"),GetLastError());
	}
	else
	{
		_tprintf(_T("----->Comparison thread have release semaphore\n"));
	}
}
	return 0;
}