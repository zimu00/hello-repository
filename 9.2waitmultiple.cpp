#define UNICODE
#define _UNICODE
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <wchar.h>


#define BUFLEN 30
#define MAXLEN 256

typedef struct Threads {
	DWORD iTh;
	HANDLE thread;
	TCHAR name[BUFLEN];
	INT i;
	INT *number;
}Threads_t;

DWORD WINAPI thSort(LPVOID para);
INT sort(INT *arrays, INT n);
INT buildFile(TCHAR *name);
void mergeArray(INT *arr1,INT n1,int *arr2,INT n2);
void copystructs(Threads_t *struct1,Threads_t *struct2);

int _tmain(INT argc, LPTSTR argv [])
{
	INT n = _tstoi(argv[1]);//?
	INT i = 0,count=0,j=0,exitTN=0;
	TCHAR buf[BUFLEN];
	TCHAR name[BUFLEN];
	HANDLE *ThreadHandle,*Handle_Copy,Handle,hOut,hInt;
	Threads_t *th,*th_copy;

	DWORD nWrt,dwRet;
	INT array_full[MAXLEN];
	INT array_full_copy[MAXLEN];

	ThreadHandle = (HANDLE *)malloc(n*sizeof(HANDLE));
	Handle_Copy = (HANDLE *)malloc(n*sizeof(HANDLE));
	th = (Threads_t *)malloc(n*sizeof(Threads_t));
	th_copy = (Threads_t *)malloc((n-1)*sizeof(Threads_t));
	for(i=0;i<n-1;i++)
	th_copy[i].number = (INT *)malloc(BUFLEN*sizeof(INT));
	i=0;

	for(;i<(n-1);i++)
	{
		fflush(stdin);//needs to use this otw will have \0 coming from previous which will be regarded
		//as filename 
		_tprintf(_T("please type %dth input file:\n"),i);
	  	_fgetts(buf,BUFLEN,stdin);//has \n,so take care
		buildFile(buf);
		th[i].iTh = i;
		/*-------------------filename------------------*/
		memset(th[i].name,0,BUFLEN);
		_tcsncpy_s(th[i].name,buf,_tcslen(buf)-1);//needs to -1 cz of \n
		_tprintf(_T("filename is %s\n"),th[i].name);
		//_tcscpy(th[i].name,buf);
		th[i].number = (INT *)malloc(BUFLEN*sizeof(INT));
		/*-------------------create thread------------------*/
		_tprintf(_T("Create thread %d\n"),th[i].iTh);
		ThreadHandle[i] = CreateThread(// _beginthreadex (???
			0,0,thSort,(PVOID)&th[i],0,&th[i].iTh);
		Handle_Copy[i] = ThreadHandle[i];
		th[i].thread = ThreadHandle[i];
	}
  i=0;
  int thAmount=n,y;
   while( 1 ){
	if(exitTN<2){
		for(i=0;i<2;i++){
			_tprintf(_T("Now waiting for handle %d"),ThreadHandle[i]);
			//dwRet=WaitForSingleObject(ThreadHandle[i],2000);
			dwRet=WaitForMultipleObjects(n-1,ThreadHandle,FALSE,INFINITE);
			dwRet=dwRet-WAIT_OBJECT_0;

			//copystructs(&th_copy[exitTN],&th[dwRet]);
			for(y=0;y<thAmount;y++)
				{
					if(th[y].thread==ThreadHandle[dwRet]){
						break;
					}
				}

			   	/*------copy first array of first two to array_full----*/
				if(exitTN==0){
					for( j=0;j<th[dwRet].i;j++){
						array_full[j]=th[y].number[j];
			   count++;
			   }
			
		     }_tprintf(_T("\n"));
				_tprintf(_T("/////before cleaning/////\n"));
		//clean the invalid handle
				for(j=0;j<n-1;j++)
			{
				_tprintf(_T("%d "),ThreadHandle[j]);
			}
				_tprintf(_T("\nI have cleaned thread %d\n"),th[dwRet].iTh);
			if(dwRet!=n-2){
				ThreadHandle[dwRet] = ThreadHandle[n-2];
		}
			for(j=0;j<n-1;j++)
			{
				_tprintf(_T("%d"),ThreadHandle[j]);
			}
			_tprintf(_T("/////after cleaning/////\n"));
		 n--;
				exitTN++;
				_tprintf(_T("========exitTN=%d\n"),exitTN);
			}
				
	//}

   }/*--------should just wait single thread-------*/
	else{

			dwRet=WaitForMultipleObjects(n-1,ThreadHandle,FALSE,INFINITE);

			dwRet = dwRet-WAIT_OBJECT_0;
			_tprintf(_T("/////thread %d ,handle %d finishes/////\n"),th[dwRet].iTh,th[dwRet].thread);
				
				exitTN++;
				_tprintf(_T("========2exitTN=%d\n"),exitTN);
			for(y=0;y<thAmount;y++)
				{
					if(th[y].thread==ThreadHandle[dwRet]){
						break;
					}
				}
		
		//clean the invalid handle
			
			if(dwRet!=n-2){
				ThreadHandle[dwRet] = ThreadHandle[n-2];
		}
	    n--;
	}	
	_tprintf(_T("\n************2Main thread waking up !!!\n"));
		
		/*-----------merge two arrays---------*/
	mergeArray(array_full,count,th[y].number,th[y].i);
		count=count+th[y].i;
	

	
   
	if(exitTN==thAmount-1)
			break;//break while cz finishes all threads
   }

	memset(buf,0,BUFLEN);
	//get outfile name
	_tprintf(_T("please type output file:\n"));
	fflush(stdin);
	_fgetts(buf,BUFLEN,stdin);
	//fflush(stdin);
	_tprintf(_T("output name is:%s\n"),buf);

	memset(name,0,BUFLEN);
	_tcsncpy_s(name,buf,_tcslen(buf)-1);
	//write into file
	hOut = CreateFile( name,GENERIC_WRITE | GENERIC_READ,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL );
	if( hOut == INVALID_HANDLE_VALUE ){
		_ftprintf(stderr,_T("Cannot open output file.Error:%x\n"),GetLastError());
		CloseHandle(hOut);
		Sleep(200);
		return 3;
	}
	//for(i=0;i<n-1;i++){
	//can write whole array or not
	WriteFile(hOut,&count,sizeof(INT),&nWrt,NULL);

	WriteFile(hOut,array_full,count*sizeof(INT),&nWrt,NULL);
	CloseHandle(hOut);
	//check result on the file
	hInt = CreateFile( name,GENERIC_WRITE | GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL );
	if( hInt == INVALID_HANDLE_VALUE ){
		_ftprintf(stderr,_T("Cannot open output file.Error:%x\n"),GetLastError());
		CloseHandle(hInt);
		Sleep(200);
		return 3;
	}
	ReadFile(hInt,array_full_copy,(count+1)*sizeof(INT),&nWrt,NULL);//needs to use count+1 
	_tprintf(_T("count=%d,result in the file: "),count);
	for(i=0;i<=count;i++)
		_tprintf(_T("%d "),array_full_copy[i]);
	//}

	free(ThreadHandle);
	CloseHandle(hInt);
	Sleep(200000);
	return 0;
}

DWORD WINAPI thSort(LPVOID para)
{
	Threads_t *th = (Threads_t *)para;
 	HANDLE h;
	//INT i;
	DWORD nr;
	INT j=0;
	OVERLAPPED ov = {0,0,0,0,NULL};

	_tprintf(_T("----in thread %d,filename is %s\n"),th->iTh,th->name);
	h=CreateFile(th->name,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(h==INVALID_HANDLE_VALUE){
	_ftprintf(stderr,_T("thread %d can't open file for reading! Error is %x\n"),th->iTh,GetLastError());
	CloseHandle(h);
	return 2;
	}  
	if(ReadFile(h,&th->i,sizeof(INT),&nr,NULL)&&(nr==sizeof(INT))){
		_tprintf(_T("thread %d,Read integer amount is:%d\n"),th->iTh,th->i);
	}
	else{
		_ftprintf(stderr,_T("thread %d Error in read\n"),th->iTh);
	}
	//th.number = malloc( (th.i+1)*sizeof(INT) );
	//read every number now
	_tprintf(_T("*****thread %d,original content is:\n"),th->iTh);
	if( ReadFile(h,th->number,th->i*sizeof(INT),&nr,NULL)&&nr==th->i*sizeof(INT) )
	{
		for(j=0;j<th->i;j++)
		//_tprintf(_T("th[%d]->number[%d]=%d "),th->iTh,j,th->number[j]);
			_tprintf(_T("%d "),th->number[j]);
	}

	//sort
	sort(th->number,th->i);
	_tprintf(_T("%d,sorted array is:"),th->iTh);
	for(j=0;j<th->i;j++){
	//_tprintf(_T("th[%d]->number[%d]=%d "),th->iTh,j,th->number[j]);
		_tprintf(_T("%d "),th->number[j]);
	}
	
	_tprintf(_T("@@@@@@@@@@@@@@%d exit\n"),GetCurrentThreadId());
	CloseHandle(h);
	ExitThread(NULL);
	return 0;
}

INT sort(INT *array, INT n){
	INT i=0,j=0,temp;
	
	_tprintf(_T("start sorting:n=%d\n"),n);
	for(j=0;j<n;j++){
	_tprintf(_T("array[%d]=%d "),j,array[j]);
	}
	for(i=0;i<n;i++)
		for(j=0;j<n-i-1;j++)//-1!!!!
		{
			if(array[j]>array[j+1])
			{
				temp=array[j];
				array[j]=array[j+1];
				array[j+1]=temp;
			}
		}
	_tprintf(_T("sort finish!\n"));
	for(j=0;j<n;j++){
	_tprintf(_T("array[%d]=%d "),j,array[j]);
	}
	return 0;
}
INT buildFile(TCHAR *name){
	HANDLE hIn;
	TCHAR number[BUFLEN];
	DWORD nIn;
	TCHAR name_t[BUFLEN];
	INT i = 0,integer=0;

	memset(name_t,0,BUFLEN);//if not clear,may have data inside
	_tprintf(_T("Create file now! name length is %d\n"),_tcslen(name)-1);
	_tprintf(_T("filename is:\n"));
	//why can't use this method???
	/*while(name[i] != '\n'){
		name_t[i]=name[i];
		i++;
		_tprintf(_T("%c"),name_t[i]);
	}*/

	wcsncpy_s(name_t,name,_tcslen(name)-1);
	_tprintf(_T("%s"),name_t);
	
	hIn = CreateFile(name_t,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hIn == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,_T("Cannot create input file %s.Error:%x\n"),name,GetLastError());
		CloseHandle(hIn);
		return 2;
	}

	_tprintf(_T("Please input integer numbers(type'\0' finish)\n"));
	while( wscanf_s(_T("%d"),&integer)==1 )
	{
	_tprintf(_T("read number: %d\n"),integer);
	WriteFile(hIn,&integer,sizeof(INT),&nIn,NULL);
	number[i] = integer;
	if(nIn==sizeof(INT))
	{
		_tprintf(_T("!!!!Successfully wrote:%d\n"),number[i]);
		i++;
	}
	else{
		_ftprintf(stderr,_T("error in writing!"));
		CloseHandle(hIn);
		Sleep(200);
		return 3;
	}
	}
	/*----------------------------------------------------------*/
	CloseHandle(hIn);
}

void mergeArray(INT *arr1,INT n1,int *arr2,INT n2){
	INT i=0,j=0,k=0;
	INT *res;

	res = (INT *)malloc( (n1+n2)*sizeof(INT) );
	while( i<n1 && j<n2 ){
		if(arr1[i] <= arr2[j]){
			res[k] = arr1[i];
			i++;
			k++;
		}else {
			res[k] = arr2[j];
			k++;
			j++;
		}
	}
	/*Some elements in array 'arr1' are still remaining where as the
	array 'arr2' is exhausted*/
	while(i<n1){
		res[k] = arr1[i];
		i++;
		k++;
	}
	/*if case is in arr2 */
	while(j<n2){
		res[k] = arr2[j];
		k++;
		j++;
	}
	/*copy res to arr1*/
	_tprintf(_T("--------------Sorted array is:----------------\n"));
	for(i=0;i<k;i++){
		arr1[i] = res[i];
		_tprintf(_T("%d "),arr1[i]);
	}
	_tprintf(_T("\n"));
}

void copystructs(Threads_t *struct1,Threads_t *struct2){
	struct1->iTh = struct2->iTh;
	struct1->i = struct2->i;
	TCHAR temp[BUFLEN];
	memset(temp,0,BUFLEN);
	_swprintf(temp,_T("%s"),struct2->name);
	_tprintf(_T("the length of name is :%d\n"),_tcslen(struct2->name));
	memset(struct1->name,0,BUFLEN);
	int s=_tcslen(struct2->name);
	for(INT kk=0;kk<s;kk++)
		struct1->name[kk] = struct2->name[kk];
	//_stprintf(struct1.name,_T("%s"),struct1.name);
	//_tcsncpy(struct1.name,struct2.name,_tcslen(struct2.name));
				
	//memset(struct1->number,0,BUFLEN);
	for(INT kk=0;kk<struct2->i;kk++)
		struct1->number[kk] = struct2->number[kk];
	struct1->i=struct2->i;
	/*-----show array------*/
	for (int j=0;j<struct1->i;j++){
	_tprintf(_T("%d "),struct1->number[j]);
	}
}
