#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#define L 128

typedef struct File{
	TCHAR directory[L];
	TCHAR input[L];
	TCHAR output[L];
}file;

int _tmain(INT argc,LPTSTR argv[]){
	File f,f1;
	HANDLE hIn,hOut;
	DWORD nW,nR;
	FILE *fp;
	INT err;

	err = _wfopen_s(&fp,argv[1],_T("r"));
	if(err !=0){
		_tprintf(_T("Cannot open output file %s.\n"),argv[1]);
		return 3;
	}
	hOut = CreateFile(argv[2],GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	hIn = CreateFile(argv[2],GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	_tprintf(_T("Debug printing"));

	while(_ftscanf(fp,_T("%s %s %s"),&f.directory,&f.input,&f.output)!=EOF){
		WriteFile(hOut,&f,sizeof(file),&nW,NULL);
		ReadFile(hIn,&f1,sizeof(file),&nR,NULL);
	    if(nR!=nW)
		{
			_tprintf(_T("Read error\n"));
			CloseHandle(hIn);
			CloseHandle(hOut);
			Sleep(1000000000);
			return 4;
		}
		_tprintf(_T("result:%s %s %s\n"),f1.directory,f1.input,f1.output);
	}
	
	CloseHandle(hIn);
	CloseHandle(hOut);
	Sleep(10000000);

	return 0;
}