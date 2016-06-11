#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_PATH 256
#define MAX_LEN 100

int _tmain(int argc,TCHAR *argv[])
{
	HANDLE hTemp;
	DWORD uRetVal,dwRetVal;
	TCHAR tempPathBuffer[MAX_PATH];
	TCHAR tempFileName[MAX_PATH];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD client;
	TCHAR str[MAX_LEN];
	DWORD n1,n2;

	client = GetCurrentProcessId();

	dwRetVal = GetTempPath(MAX_PATH,
		tempPathBuffer);
	if(dwRetVal>MAX_PATH || (dwRetVal == 0))
	{
		_ftprintf(stderr,_T("GetTempPath failed.Error:%x\n"),GetLastError());
	}
	uRetVal = GetTempFileName(tempPathBuffer,
		TEXT("TEMP"),
		0,
		tempFileName);
	if(uRetVal == 0)
	{
		_tprintf(_T("GetTempFileName failed.Error:%x\n"),GetLastError());
		CloseHandle(hTemp);
		return -1;
	}
	//start the client 
	if(CreateProcess( NULL,
		tempFileName,//filename
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
			return;
	}

	//client
	if(GetCurrentProcessId() == client){
	//read from stdin
		HANDLE h;
		h= CreateFile(tempFileName,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,NULL);
		if(h == INVALID_HANDLE_VALUE)
		{
			_tprintf(_T("Create file failed for client.Error:%x\n"),GetLastError());
			return -1;
		}
		while ( _tcscmp(str,_T("end-of-file"))!=0 )//ctrl-z
		{
			memset(str,0,MAX_LEN);
			_fgetts(str,MAX_LEN,stdin);
			WriteFile(h,str,MAX_LEN,&n1,NULL);
		}
		memset(str,0,MAX_LEN);
		str[0] = '\0';
		_tcscpy(str,".end");
		WriteFile(h,str,MAX_LEN,&n1,NULL);





	}
}