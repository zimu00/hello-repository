#pragma warning(disable:4996)
#define _CRT_non_CONFORMING_SWPRINTFS
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_LEN 1024

int _tmain(int argc,LPTSTR argv[]){

	HANDLE hStdin,hStdout;
	TCHAR buf[BUF_LEN];
	DWORD dRead,dWrite;
	BOOL success;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);

	if( (hStdin==INVALID_HANDLE_VALUE)||(hStdout==INVALID_HANDLE_VALUE) ){
		_tprintf(_T("Error:%x\n"),GetLastError());
		Sleep(1000000);
		ExitProcess(1);}

	success=ReadFile(hStdin,buf,BUF_LEN,&dRead,NULL);
	if(success==0){
		_tprintf(_T("read failed\n"));
		Sleep(1000000);
		CloseHandle(hStdin);
		CloseHandle(hStdout);
		exit(-1);
	}
	success=WriteFile(hStdout,buf,dRead,&dRead,NULL);
	if(success==0){
		_tprintf(_T("write failed\n"));
		Sleep(1000000);
		CloseHandle(hStdin);
		CloseHandle(hStdout);
		exit(-1);
	}

	CloseHandle(hStdin);
		CloseHandle(hStdout);
	return 0;
}