#pragma warning(disable:4996)
#define _CRT_NON_CONFORMING_SWPRINTFS

#define UNICODE
#define _UNICODE

#define NAME 200

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Records{
	INT row;
	DWORD id;
	TCHAR name[NAME];
	TCHAR surname[NAME];
	DWORD amount;
}Record;

int _tmain(int argc,LPTSTR argv []){
FILE *fd;
	HANDLE hIn,hOut;
	DWORD nIn,nOut,i;
	Record record;
	TCHAR fileName[NAME];

	_tcscpy(fileName,argv[1]);
	fd = _wfopen(fileName,_T("r"));
	if(fd == NULL){
		_tprintf(_T("Cannot open output file %s\n"),fileName);
		return 1;
	}
	hOut = CreateFile(argv[2],GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hOut == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot open output file.Error:%x\n"),GetLastError());
		CloseHandle(hOut);
		return 3;
	}

	 _tprintf(_T("Debug Printing 1 (what I read from ASCCI and write to bin):\n"));
	 while(_ftscanf(fd,_T("%d%ld%s%s%d"),&record.row, &record.id, record.surname, record.name, &record.amount)!=EOF){
		 WriteFile(hOut,&record,sizeof(record),&nOut,NULL);
		 
	 }

	fclose(fd);
	CloseHandle(hOut);

	 hIn = CreateFile (argv[2], GENERIC_READ, FILE_SHARE_READ, NULL,
    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hIn == INVALID_HANDLE_VALUE) {
    _ftprintf (stderr, _T("Cannot open input file. Error: %x\n"), GetLastError ());
	CloseHandle(hIn);
	Sleep(20000);
    return 2;
  }

	 printf("check the result:\n");
  //reread the file
   while (ReadFile (hIn, &record, sizeof(record), &nIn, NULL) && nIn > 0) {
    //WriteFile (hOut, &record, nIn, &nOut, NULL);
		if (nIn != sizeof(record)) {
			_ftprintf (stderr, _T("Fatal write error: %x\n"), GetLastError ());
			CloseHandle(hIn); 
			return 4;
		}
		else//no &
			_tprintf(_T("%d %ld %s %s %d"),record.row, record.id, record.surname, record.name, record.amount);
  }


  CloseHandle (hIn);

  Sleep(100000000);
}