//G:\lab\output1.bin 1 1
#pragma warning(disable:4996)
#define _CRT_non_CONFORMING_SWPRINTFS
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#define L 128
#define newLine 0x0D0A

typedef struct Records{
	DWORD count;
	DWORD character[L];
	DWORD line[L];
	LPTSTR filename[L];
}record;

typedef struct File{
	TCHAR directory[L];
	TCHAR input[L];
	TCHAR output[L];
}file;

INT outfile_num=0;

HANDLE Gse;
INT finish;

typedef struct outputs{
	TCHAR output[L];
	INT output_num;
	INT flag;
}Output;

typedef struct  Threads_t
{
	HANDLE h;
	file f;
}Thread;

CRITICAL_SECTION cs,cs1,cs2,cs3;
INT GFlag = 0,M,N;

Output out[L];

DWORD WINAPI Total(LPVOID);
DWORD WINAPI Compute(LPVOID);

int _tmain(int argc,LPTSTR argv[]){
	Thread thread;
	INT i;
	HANDLE *th,h;
	
	if(argc!=4)
	{
		_tprintf(_T("syntax error\n"));
		Sleep(1000000000);
		exit(0);
	}

	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&cs1);
	InitializeCriticalSection(&cs2);
	InitializeCriticalSection(&cs3);

	N = _tstoi(argv[2]);
	M = _tstoi(argv[3]);
	th = (HANDLE *)malloc((N+1)*sizeof(HANDLE));

	File *f;
	f = (file *)malloc(N*sizeof(file));
	/*for(i=0;i<N;i++){
		out[i].se = CreateSemaphore(NULL,0,L,NULL);
	}*/
	Gse = CreateSemaphore(NULL,0,L,NULL);

	EnterCriticalSection(&cs3);
			finish =0;
	LeaveCriticalSection(&cs3);

	h = CreateFile(argv[1],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(h == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot open input file.Error:%x\n"),GetLastError());
		CloseHandle(h);
		Sleep(1000000000);
		return 1;
	}

	for(i=0;i<N;i++){
		thread.h = h;
		thread.f =f[i];
		th[i] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Compute,(LPVOID)&thread,0,NULL);
		if(th[i]==NULL){
			_tprintf(_T("Error in creating thread"));
			return 2;
		}
	}

	th[i] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Total,NULL,0,NULL);

	WaitForMultipleObjects(N+1,th,TRUE,INFINITE);
	DeleteCriticalSection(&cs);
	DeleteCriticalSection(&cs1);
	DeleteCriticalSection(&cs2);
	CloseHandle(h);
	free(th);
	for(i=0;i<N+1;i++){
		CloseHandle(th[i]);
	}

	return 0;
}

DWORD WINAPI Compute(LPVOID param){

	record record_th;
	Thread th = *(Thread *)param;
	TCHAR a;
	DWORD r;
	INT i=0;
	DWORD n,nw;
	HANDLE SearchHandle;
	WIN32_FIND_DATA FindData;
	LPTSTR name;
	HANDLE hIn,hOut;
	TCHAR filetmp[L][L];
	TCHAR filet[L];
	INT tmp_c=0;

	record_th.count = 0;
	for(;i<L;i++){
	//record_th.filename[i]=NULL;//can't be printed

	record_th.character[i]=0;
	record_th.line[i] = 0;
	}
	TCHAR fi[L+L];
	
	while(1){
		EnterCriticalSection(&cs2);
					if(out[0].output_num == -1){
						_tprintf(_T("finish,%d exit\n"),GetThreadId);
						break;}
		LeaveCriticalSection(&cs2);

		EnterCriticalSection(&cs);

		ReadFile(th.h,&th.f,sizeof(File),&n,NULL);
		
		LeaveCriticalSection(&cs);
	
		if(n==0){
			EnterCriticalSection(&cs3);
			finish ++;
			LeaveCriticalSection(&cs3);
			
			//EnterCriticalSection(&cs2);
					//for(i=0;i<outfile_num;i++){
					//	out[i].output_num = -1;
						
			//ReleaseSemaphore(Gse,1,NULL);
					//}
			LeaveCriticalSection(&cs2);
			break;
		}
		else{
		_tprintf(_T("\n%d will visit directory %s\n"),GetThreadId(NULL),th.f.directory);
			hOut = CreateFile(th.f.output,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			if(hOut==NULL){
				_ftprintf(stderr,_T("threads Cannot open input file.Error:%x\n"),GetLastError());
				CloseHandle(hOut);
				Sleep(1000000000);
				return 1;
			}
			OVERLAPPED ov = {0,0,0,0,NULL};
			LARGE_INTEGER filePos;
			_stprintf(fi,_T("%s\\%s"),th.f.directory,th.f.input);
			SearchHandle = FindFirstFile(fi,&FindData);
			if(SearchHandle == INVALID_HANDLE_VALUE){
				_tprintf(_T("find %s fail %x\n"),fi,GetLastError());
				Sleep(10000000000);
				return 4;
			}
			do{
				//avoid ./..
				if(lstrcmp(FindData.cFileName,_T("."))!=0 && lstrcmp(FindData.cFileName,_T(".."))!=0){
				//record_th.filename[record_th.count][0]='\0';
				filetmp[tmp_c][0]='\0';
				_stprintf(filetmp[tmp_c],_T("%s\\%s"),th.f.directory,FindData.cFileName);
				
				//filetmp[tmp_c]=strdup(filet);
				record_th.filename[record_th.count]=filetmp[tmp_c];
				//record_th.filename[record_th.count]=filetmp;
				//_stprintf(record_th.filename[record_th.count],_T("%s\\%s"),th.f.directory,FindData.cFileName);
				//hIn = CreateFile(FindData.cFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				hIn = CreateFile(filetmp[tmp_c],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if(hIn==INVALID_HANDLE_VALUE){
					_tprintf(_T("Error in handle ,%s Error:%x\n"),filetmp[tmp_c],GetLastError());
					Sleep(100000000000);
					return 5;
				}
				tmp_c++;
				//GetFileSizeEx(hIn,(PLARGE_INTEGER)record_th.character[record_th.count]);
				//if(IsTextUnicode())
				while(ReadFile(hIn,&a,sizeof(TCHAR),&nw,NULL)&&nw){
					//if(a==newLine){
					if(a=='\r\n'){	
						record_th.line[record_th.count]++;
					}
					record_th.character[record_th.count]++;
				}
					
				record_th.count++;//add everytime find a new file
				}
			}while(FindNextFile(SearchHandle,&FindData));

			EnterCriticalSection(&cs1);
					r = GetFileSize(hOut,NULL);
					filePos.QuadPart = r;
					ov.Offset =filePos.LowPart;
					ov.OffsetHigh = filePos.HighPart;
					//_tprintf(_T("Write:%s %d %d %d\n"),record_th.filename,record_th.character,record_th.count,record_th.line);
					_tprintf(_T("I write!\n"));
					
					WriteFile(hOut,&record_th,sizeof(record),&nw,&ov);
					//update record_th.count
					record_th.count =0;

					record r_tmp;
					INT l;
					ReadFile(hOut,&r_tmp,sizeof(record),&nw,&ov);
					_tprintf(_T("\nha %d "),r_tmp.count);
				for(l=0;l<r_tmp.count;l++)
				_tprintf(_T("ha %s %d %d\n"),r_tmp.filename[l],r_tmp.character[l],r_tmp.line[l]);
					LeaveCriticalSection(&cs1);

			INT flag = 0;
					EnterCriticalSection(&cs2);
					for(i=0;i<outfile_num;i++){
						if(_tcscmp(out[i].output,th.f.output)==0){
							flag = 1;//n ot ==
							break;
						}
					}
					if(flag == 0){
						//out[i].output = th.f.output;
						out[i].output[0] ='\0';
						_tcscpy(out[i].output,th.f.output);
						out[i].flag = 0;//used for total thread to check it has been opened or not,then only need to print new records
						outfile_num ++;
					}
					out[i].output_num ++;
					if(out[i].output_num==M)
					{
						ReleaseSemaphore(Gse,1,NULL);
						//out[i].output_num ==0;
					}
					LeaveCriticalSection(&cs2);

		}
		CloseHandle(hIn);
		CloseHandle(hOut);
	}


	return 0;
}

DWORD WINAPI Total(LPVOID param){
	INT i,j=0,k=0,l;
	record r;
	HANDLE h[L];
	DWORD n;
	INT count=0;

	while(1){
	
		
		WaitForSingleObject(Gse,INFINITE);
		
		EnterCriticalSection(&cs3);
		if(finish==N){
				_tprintf(_T("all are finished!\n"));
				LeaveCriticalSection(&cs3);
				ExitThread(NULL);
				//Sleep(100000);
		//		exit(0);
		}
		LeaveCriticalSection(&cs3);

		EnterCriticalSection(&cs2);
		for(i=0;i<outfile_num;i++){
			/*if(out[i].output_num==-1){
				_tprintf(_T("all are finished!\n"));
				//ExitThread(NULL);
				Sleep(100000);
				exit(0);
			}*/

			if(out[i].output_num==M){
				if(out[i].flag==0){
				h[i]=CreateFile(out[i].output,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				if(h[i]==INVALID_HANDLE_VALUE){
					_tprintf(_T("error in handle.Error:%x\n"),GetLastError());
				}
				out[i].flag=1;
				}

			for(k=0;k<M;k++){
				ReadFile(h[i],&r,sizeof(record),&n,NULL);
				if(n!=sizeof(record)){
					_tprintf(_T("error in checking Error%x\n"),GetLastError());
					Sleep(100000000);
					return 8;
				}
				_tprintf(_T("hihi %d "),r.count);
				for(l=0;l<r.count;l++)
				_tprintf(_T("hihi %s %d %d\n"),r.filename[l],r.character[l],r.line[l]);
			}
			out[i].output_num=0;}
		}
		LeaveCriticalSection(&cs2);
	}

	return 0;
}