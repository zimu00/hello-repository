#pragma warning(disable:4996)
#define _CRT_non_CONFORMING_SWPRINTFS
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFLEN 4096

DWORD number[BUFLEN];

typedef struct Threads_t{
	DWORD thId;
}threads;

DWORD WINAPI check1(LPTSTR);
DWORD WINAPI check2(LPTSTR);
DWORD Filter (LPEXCEPTION_POINTERS pExP,LPDWORD ECatgry);
int createFile(LPTSTR file);

int _tmain(int argc,LPTSTR argv []){
	
	threads th[2];
	HANDLE ThreadHandle[2];
	INT i=0;
	
	//createFile(argv[1]);
	ThreadHandle[0] = CreateThread(0,0,(LPTHREAD_START_ROUTINE)check1,argv[1],0,&th[0].thId);
	ThreadHandle[1] = CreateThread(0,0,(LPTHREAD_START_ROUTINE)check2,argv[1],0,&th[1].thId);

	WaitForMultipleObjects(2,ThreadHandle,TRUE,INFINITE);
	for(;i<2;i++){
		CloseHandle(ThreadHandle[i]);
	}

	return 0;
}

DWORD WINAPI check1(LPTSTR param){
	DWORD ECatgry;
	HANDLE h;
	DWORD nIn,i=0;
	BOOL Done = FALSE;
	DWORD buf[BUFLEN];

	//while(!Done)
	_try{ 
		_try { /*Try-Except block*/
			
				h = CreateFile(param,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if (h == INVALID_HANDLE_VALUE)
				{
					_tprintf(_T("test:%d\n"),GetLastError());
					RaiseException(0XE0000006,EXCEPTION_NONCONTINUABLE,NULL,NULL);
				}
				while(ReadFile(h,&buf[i],sizeof(DWORD),&nIn,NULL)&&nIn>0){
				i++;
				if(i%2==0){
					_tprintf(_T("n1=%d,n2=%d\n"),buf[i-2],buf[i-1]);
					_tprintf(_T("n1/n2=%d\n"),buf[i-2]/buf[i-1]);
				 }
				}
				//check the file has odd number or even number
				if(i%2!=0)
				{
					RaiseException(0XE0000007,EXCEPTION_NONCONTINUABLE,NULL,NULL);
				}
			}

		_except (Filter (GetExceptionInformation (),&ECatgry)){
			_tprintf(_T("I'm in filter\n"));
			switch (ECatgry){
			case 1:_tprintf(_T("invalid file exception\n"));
				//if(no_exist_h == INVALID_HANDLE_VALUE)
				//_ftprintf(stderr,_T("Cannot open input file:Error:%x\n"),GetLastError());
				break;
			case 2:_tprintf(_T("Can't divide 0!"));
				break;
			case 3:_tprintf(_T(""));
				break;
			case 10:_tprintf(_T("User generated exception.\n"));
				break;
			default:_tprintf(_T("Unknown exception\n"));
				break;
			
			}
			_tprintf(_T("End of handler.\n"));
		}
	}
	__finally {//try...finally
		BOOL AbTerm;
		AbTerm = AbnormalTermination();
		_tprintf(_T("Abnormal Termination?: %d\n"),!AbTerm);
	}
	return 0;
}

DWORD WINAPI check2(LPTSTR param){

	DWORD ECatgry;
	HANDLE h;
	DWORD nIn,i=0;
	INT *buf;
	INT temp;

	 _try{ 
		_try { /*Try-Except block*/
			
				h = CreateFile(param,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if (h == INVALID_HANDLE_VALUE)
				{
					_tprintf(_T("test:%d\n"),GetLastError());
					RaiseException(0XE0000006,EXCEPTION_NONCONTINUABLE,NULL,NULL);
				}
				ReadFile(h,&temp,sizeof(DWORD),&nIn,NULL);
				_tprintf(_T("%d "),temp);
				if(temp<0)
				{
					RaiseException(0XE0000008,EXCEPTION_NONCONTINUABLE,NULL,NULL);
				}

				buf = (INT *)malloc(temp*sizeof(INT));
				
				buf[i]=temp;
				i++;
				while(ReadFile(h,&buf[i],sizeof(DWORD),&nIn,NULL)&&nIn>0){
					_tprintf(_T("%d "),buf[i]);
				i++;
				if(i>=temp)
					RaiseException(0XE0000010,EXCEPTION_NONCONTINUABLE,NULL,NULL);
				}
				//check the file has odd number or even number
			}

		_except (Filter (GetExceptionInformation (),&ECatgry)){
			_tprintf(_T("I'm in filter\n"));
			switch (ECatgry){
			case 1:_tprintf(_T("invalid file exception\n"));
				//if(no_exist_h == INVALID_HANDLE_VALUE)
				//_ftprintf(stderr,_T("Cannot open input file:Error:%x\n"),GetLastError());
				//break;
			case 2:_tprintf(_T("Can't divide 0!"));
				//break;
			case 3:_tprintf(_T("Memory access violation.\n"));
				//break;
			case 4:_tprintf(_T("Wrong memory allocation.\n"));
				//break;
			case 5:_tprintf(_T("array bound exception\n"));
				//break;
			case 10:_tprintf(_T("User generated exception.\n"));
				//break;
			default:_tprintf(_T("Unknown exception\n"));
				break;
			
			}
			_tprintf(_T("End of handler.\n"));
		}
	}
	__finally {//try...finalally
		BOOL AbTerm;
		AbTerm = AbnormalTermination();
		_tprintf(_T("Abnormal Termination?: %d\n"),!AbTerm);
	}
	return 0;
}

DWORD Filter (LPEXCEPTION_POINTERS pExP,LPDWORD ECatgry){
	DWORD ExCode;
	DWORD_PTR ReadWrite,VirtAddr;
	ExCode = pExP->ExceptionRecord->ExceptionCode;
	_tprintf(_T("Filter.ExCode:%x\n"),ExCode);
	if((0x200000000 & ExCode)!=0){/*user exception*/
		if(ExCode == 0XE0000008)
			*ECatgry = 4;
		else if(ExCode == 0XE0000010)
			*ECatgry = 5;
		*ECatgry = 10;
		return EXCEPTION_EXECUTE_HANDLER;
	}

	switch (ExCode){
	case ERROR_INVALID_HANDLE:
		*ECatgry = 1;
		return EXCEPTION_EXECUTE_HANDLER;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		*ECatgry = 2;
		return EXCEPTION_EXECUTE_HANDLER;
	case EXCEPTION_ACCESS_VIOLATION:
				/* Determine whether it was a read or write
					and give the virtual address. */
			ReadWrite =
				pExP->ExceptionRecord->ExceptionInformation [0];
			VirtAddr =
				pExP->ExceptionRecord->ExceptionInformation [1];
			_tprintf
				(_T("Access Violation. Read/Write: %d. Address: %x\n"),
				ReadWrite, VirtAddr);
			*ECatgry = 3;
			return EXCEPTION_EXECUTE_HANDLER;
	
	default:
			*ECatgry = 0;
			return EXCEPTION_CONTINUE_SEARCH;
	}
}

int createFile(LPTSTR file){
	_tprintf(_T("Please input integer numbers(separated with space and terminated with '-1')"));
	INT i=0,j;
	HANDLE hIn,hOut;
	DWORD nWrt,nRd;
	DWORD buf[BUFLEN];

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