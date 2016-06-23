#pragma warning(disable:4996)
#define _CRT_non_CONFORMING_SWPRINTFS
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_LEN 1024

typedef struct con{
	INT num;
	LPTSTR a;
}con;

int _tmain(int argc,LPTSTR argv[]){

	HANDLE hStdin,hStdout;
	TCHAR buf[BUF_LEN];
	DWORD dRead,dWrite;
	BOOL success;
	con c;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);

	if( (hStdin==INVALID_HANDLE_VALUE)||(hStdout==INVALID_HANDLE_VALUE) ){
		_tprintf(_T("Error:%x\n"),GetLastError());
		Sleep(1000000);
		ExitProcess(1);}

	success=ReadFile(hStdin,&c,sizeof(c),&dRead,NULL);
	//success=ReadFile(hStdin,buf,BUF_LEN,&dRead,NULL);
	if(success==0){
		_tprintf(_T("read failed\n"));
		Sleep(1000000);
		CloseHandle(hStdin);
		CloseHandle(hStdout);
		exit(-1);
	}
	//_tprintf(_T("read bytes:%d"),dRead);//can't have this in the middle ,then writefile can't work,it should
	//be due to redirection of stdout

	success=WriteFile(hStdout,&c,dRead,&dRead,NULL);
	if(success==0){
		_tprintf(_T("write failed\n"));
		Sleep(1000000);
		CloseHandle(hStdin);
		CloseHandle(hStdout);
		exit(-1);
	}
	Sleep(100000);
	CloseHandle(hStdin);
		CloseHandle(hStdout);
	return 0;
}