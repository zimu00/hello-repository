
#pragma warning(disable:4996)
#define _CRT_non_CONFORMING_SWPRINTFS
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_LEN 100
#define CTR_Z 0X1A

void client(LPTSTR);
void server(LPTSTR);

int _tmain(int argc,TCHAR *argv[])
{
	if(argc == 1){
		client(argv[0]);
	}
	else{
	int i = 0;
	for(;i<argc;i++)
		_tprintf(_T("%s "),argv[i]);
	server(argv[1]);
	}

	
		
	return 0;	
}


void server(LPTSTR tempFileName)	
	{
		HANDLE hI;
		TCHAR read[MAX_LEN];
		DWORD n2;
		DWORD server;

		server = GetCurrentProcessId();
		_tprintf(_T("Hi,I'm server %d\n"),server);
		
		hI = CreateFile(tempFileName,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,
			OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hI == INVALID_HANDLE_VALUE) {
			_ftprintf(stderr,_T("Server cannot open input file %s.Error:%x\n"),tempFileName,GetLastError());
			CloseHandle(hI);
			return;
		}

		while ( _tcscmp(read,_T(".end"))!=0 )
		{
			memset(read,0,MAX_LEN);
			ReadFile(hI,read,MAX_LEN,&n2,NULL);
			if (n2 == MAX_LEN)
				_tprintf(_T("I'm server reading:%s\n"),read);
		}
		
		CloseHandle(hI);
	}

void client(LPTSTR param){
	HANDLE hTemp;
	DWORD uRetVal,dwRetVal;
	TCHAR tempPathBuffer[MAX_PATH];
	TCHAR tempFileName[MAX_PATH];
	TCHAR executable[MAX_PATH];
	TCHAR arguments[MAX_PATH];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD client,server;
	TCHAR str[MAX_LEN],str_temp[MAX_LEN];
	DWORD n1;
	OVERLAPPED ov = {0,0,0,0,NULL};
	LARGE_INTEGER filePos,fileReserved;

	client = GetCurrentProcessId();
	_tprintf(_T("id is %d\n"),client);

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
		return ;
	}
	//create the file using the filename
	 hTemp = CreateFile(tempFileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,NULL);
		if(hTemp == INVALID_HANDLE_VALUE)
		{
			_tprintf(_T("Create file failed for client.Error:%x\n"),GetLastError());
			return ;
		}
		//CloseHandle(hTemp);
	//start the client 
		// the executable file name (argv[0]) can have some spaces: sourround it with quotes
		_stprintf(executable,_T("\"%s\""),param);//????
		//_stprintf(arguments,_T("%s %s"),executable,tempFileName);
		_stprintf(arguments,_T("%s %s"),param,tempFileName);
		GetStartupInfo(&si);
		
	//if(!CreateProcess(param,
		if(!CreateProcess( NULL,
		arguments,//filename as parameter
		NULL,        //Process handle not inheritable
		NULL,		 //Thread handle not inheritable
		TRUE,		//Set handle inheritance to TRUE
		0,			//NO creation flags
		NULL,		//Use parent's environment block
		NULL,		//Use parent's starting directory
		&si,		//Pointer to STARTUPINFO structure
		&pi)		//Pointer to PROCESS_INFORMATION
		){
			_tprintf(_T("CreateProcess failed(%d).\n"),GetLastError());
			return ;
	}
		server = pi.dwProcessId;
		_tprintf(_T("server is %d\n"),server);
	_tprintf(_T("%d finishes!\n"),GetCurrentProcessId());

	//read from stdin
		//HANDLE h;
		DWORD line=1;
		DWORD lineLength;
		DWORD nRead;
		/*h= CreateFile(tempFileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,NULL);
		if(h == INVALID_HANDLE_VALUE)
		{
			_tprintf(_T("Create file failed for client.Error:%x\n"),GetLastError());
			return -1;
		}*/

		//set uo console in order to stop reading when user types CTR_Z
		CONSOLE_READCONSOLE_CONTROL rcc;
		rcc.nLength = sizeof(CONSOLE_READCONSOLE_CONTROL);
		rcc.nInitialChars = 0;
		rcc.dwCtrlWakeupMask = 1 <<CTR_Z;
		rcc.dwControlKeyState = 0;

		fileReserved.QuadPart = MAX_LEN*sizeof(TCHAR);
			filePos.QuadPart = (line-1)*MAX_LEN*sizeof(TCHAR);
			ov.Offset = filePos.LowPart;
			ov.OffsetHigh = filePos.HighPart;
			ov.hEvent = 0;
			bool k=LockFileEx(hTemp,LOCKFILE_EXCLUSIVE_LOCK,0,fileReserved.LowPart,fileReserved.HighPart,&ov);
			if(k==false)
				_tprintf(_T("error in locking file:%x\n"),GetLastError());
		while ( 1 )//ctrl-z
		{
			memset(str,0,MAX_LEN);

			_tprintf(_T("Please input a string(CTR_Z to stop)\n"));
			ReadConsole(GetStdHandle(STD_INPUT_HANDLE),str,MAX_LEN-3,&nRead,&rcc);
			
			str[MAX_LEN-3] = _T('\r');
			str[MAX_LEN-2] = _T('\n');
			str[MAX_LEN-1] = _T('\0');
			lineLength = _tcsnlen(str,MAX_LEN);
			if(str[lineLength-3]==CTR_Z){
				_tprintf(_T("found CTRL-Z\n"));
			//if(_tstoi(str_check)==EOF)
				break;}
				//return 0;
			WriteFile(hTemp,str,MAX_LEN*sizeof(TCHAR),&n1,&ov);
			if(n1 != MAX_LEN*(sizeof(TCHAR))){
				_tprintf(_T("error:%x\n"),GetLastError()); 
				UnlockFileEx(hTemp,0,fileReserved.LowPart,fileReserved.HighPart,&ov);
				break;
			}
			else
			{
			line ++;
			filePos.QuadPart = (line-1)*MAX_LEN*sizeof(TCHAR);
			ov.Offset = filePos.LowPart;
			ov.OffsetHigh = filePos.HighPart;
			LockFileEx(hTemp,LOCKFILE_EXCLUSIVE_LOCK,0,fileReserved.LowPart,fileReserved.HighPart,&ov);
			
			filePos.QuadPart = (line-2)*MAX_LEN*sizeof(TCHAR);
			ov.Offset = filePos.LowPart;
			ov.OffsetHigh = filePos.HighPart;
			UnlockFileEx(hTemp,0,fileReserved.LowPart,fileReserved.HighPart,&ov);

			filePos.QuadPart = (line-1)*MAX_LEN*sizeof(TCHAR);
			ov.Offset = filePos.LowPart;
			ov.OffsetHigh = filePos.HighPart;
			}
		}
		memset(str,0,MAX_LEN);
		str[0] = '\0';
		_tcscpy(str,".end");
		WriteFile(hTemp,str,MAX_LEN*sizeof(TCHAR),&n1,&ov);
		UnlockFileEx(hTemp,0,fileReserved.LowPart,fileReserved.HighPart,&ov);

		CloseHandle(hTemp);

}