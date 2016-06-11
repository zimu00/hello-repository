
#pragma warning(disable:4996)
#define _CRT_non_CONFORMING_SWPRINTFS
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_LEN 100

int _tmain(int argc,TCHAR *argv[])
{
	HANDLE hTemp;
	DWORD uRetVal,dwRetVal;
	TCHAR tempPathBuffer[MAX_PATH];
	TCHAR tempFileName[MAX_PATH];
	TCHAR executable[MAX_PATH];
	TCHAR arguments[MAX_PATH];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD client;
	TCHAR str[MAX_LEN],str_temp[MAX_LEN];
	DWORD n1,n2;
	OVERLAPPED ov = {0,0,0,0,NULL};
	LARGE_INTEGER filePos,fileReserved;

	client = GetCurrentProcessId();

	ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	/*dwRetVal = GetTempPath(MAX_PATH,
		tempPathBuffer);
	if(dwRetVal>MAX_PATH || (dwRetVal == 0))
	{
		_ftprintf(stderr,_T("GetTempPath failed.Error:%x\n"),GetLastError());
	}*/
	uRetVal = GetTempFileName(_T("."),
		_T("lab1203"),
		0,
		tempFileName);
	if(uRetVal == 0)
	{
		_tprintf(_T("GetTempFileName failed.Error:%x\n"),GetLastError());
		return -1;
	}
	//create the file using the filename
	 hTemp = CreateFile(tempFileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,NULL);
		if(hTemp == INVALID_HANDLE_VALUE)
		{
			_tprintf(_T("Create file failed for client.Error:%x\n"),GetLastError());
			return -1;
		}
		CloseHandle(hTemp);
	//start the client 

		_stprintf(executable,_T("\"%s\""),argv[0]);//????
		_stprintf(arguments,_T("%s %s"),executable,tempFileName);
		GetStartupInfo(&si);

	if(!CreateProcess( argv[0],
		arguments,//filename as parameter
		NULL,        //Process handle not inheritable
		NULL,		 //Thread handle not inheritable
		FALSE,		//Set handle inheritance to FALSE
		0,			//NO creation flags
		NULL,		//Use parent's environment block
		NULL,		//Use parent's starting directory
		&si,		//Pointer to STARTUPINFO structure
		&pi)		//Pointer to PROCESS_INFORMATION
		){
			_tprintf(_T("CreateProcess failed(%d).\n"),GetLastError());
			return -1;
	}

	//client
	if(GetCurrentProcessId() == client){
	//read from stdin
		HANDLE h;
		DWORD line=1;
		TCHAR str_check[MAX_LEN];
		h= CreateFile(tempFileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,NULL);
		if(h == INVALID_HANDLE_VALUE)
		{
			_tprintf(_T("Create file failed for client.Error:%x\n"),GetLastError());
			return -1;
		}

		fileReserved.QuadPart = MAX_LEN*sizeof(TCHAR);
			filePos.QuadPart = (line-1)*MAX_LEN*sizeof(TCHAR);
			ov.Offset = filePos.LowPart;
			ov.OffsetHigh = filePos.HighPart;
			ov.hEvent = 0;
			bool k=LockFileEx(h,LOCKFILE_EXCLUSIVE_LOCK,0,fileReserved.LowPart,fileReserved.HighPart,&ov);
			if(k==false)
				_tprintf(_T("error in locking file:%x\n"),GetLastError());
		while ( 1 )//ctrl-z
		{
			memset(str,0,MAX_LEN);
			memset(str_temp,0,MAX_LEN);
			memset(str_check,0,MAX_LEN);
			str_temp[0]='\0'; 
			_tprintf(_T("Please input a string\n"));
			Sleep(1000000);
			_fgetts(str_temp,MAX_LEN*sizeof(TCHAR),stdin);
			_tcsncpy(str,str_temp,strlen(str_temp)-1);
			//if( _tcscmp(str,_T("end-of-file"))==0 )
			_tcscat(str_check,str);
			if(_tstoi(str_check)==EOF)
				break;
			WriteFile(h,str,MAX_LEN*sizeof(TCHAR),&n1,&ov);
			if(n1 != MAX_LEN*(sizeof(TCHAR)))
				_tprintf(_T("error:%x\n"),GetLastError());
			line ++;
			filePos.QuadPart = (line-1)*MAX_LEN*sizeof(TCHAR);
			ov.Offset = filePos.LowPart;
			ov.OffsetHigh = filePos.HighPart;
			LockFileEx(h,LOCKFILE_EXCLUSIVE_LOCK,0,fileReserved.LowPart,fileReserved.HighPart,&ov);
			filePos.QuadPart = (line-2)*MAX_LEN*sizeof(TCHAR);
			UnlockFileEx(h,0,fileReserved.LowPart,fileReserved.HighPart,&ov);
		}
		memset(str,0,MAX_LEN);
		str[0] = '\0';
		_tcscpy(str,".end");
		WriteFile(h,str,MAX_LEN*sizeof(TCHAR),&n1,NULL);
		UnlockFileEx(h,0,fileReserved.LowPart,fileReserved.HighPart,&ov);

		CloseHandle(h);

	}
	else{
		HANDLE hI;
		TCHAR read[MAX_LEN];
		
		hI = CreateFile(tempFileName,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,
			OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hI == INVALID_HANDLE_VALUE) {
			_ftprintf(stderr,_T("Cannot open input file.Error:%x\n"));
			CloseHandle(hI);
			return 2;
		}

		while ( _tcscmp(read,_T(".end"))!=0 )
		{
			memset(read,0,MAX_LEN);
			ReadFile(hI,read,MAX_LEN,&n2,NULL);
			if (n2 == MAX_LEN)
				_tprintf(_T("%s\n"),read);
		}
		
		CloseHandle(hI);
	}

	_tprintf(_T("%d finishes!\n"),GetCurrentProcessId());
	return 0;
}