#define _CRT_NON_CONFORMING_SWPRINTFS
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#define L 500

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_DOT 3

void TraverseDirectoryRecursive(LPTSTR,LPTSTR,LPTSTR);
DWORD FileType (LPWIN32_FIND_DATA pFileData);

int _tmain (int argc, LPTSTR argv [])
{
	TCHAR path[L],fullPath[L];

	_tprintf(_T("\n--> Visit Dir %s\n"),argv[1]);
	GetCurrentDirectory(L,path);//??
	_stprintf(fullPath,_T("%s\\%s"),path,argv[2]);
	TraverseDirectoryRecursive(path,argv[1],fullPath);



	return 0;
}

void TraverseDirectoryRecursive (LPTSTR path,LPTSTR SourcePathName,LPTSTR FullDestPathName)//use of level?
{
	HANDLE SearchHandle;
	WIN32_FIND_DATA FindData;
	DWORD FType, l;
	TCHAR fullNewPath[L];

	SetCurrentDirectory(SourcePathName);

	SearchHandle = FindFirstFile(_T("*"),&FindData);

	do {
		FType = FileType(&FindData);

		l = _tcslen(FullDestPathName);
		if(FullDestPathName[l-1] == '\\'){
			_stprintf(fullNewPath,_T("%s%s"),FullDestPathName,FindData.cFileName);
		}else{
			_stprintf(fullNewPath,_T("%s%s"),FullDestPathName,FindData.cFileName);
		}

		if(FType == TYPE_FILE){
			_tprintf(_T("-->Copy FILE: %s %s\n"),FindData.cFileName,fullNewPath);
			CopyFile(FindData.cFileName,fullNewPath,FALSE);/*If this parameter is TRUE 
		and the new file specified by lpNewFileName already exists, the function fails. If this parameter
		is FALSE and the new file already exists, the function overwrites the existing file and succeeds.*/
		}
		if(FType == TYPE_DIR){
			TraverseDirectoryRecursive(path, FindData.cFileName,fullNewPath);
			SetCurrentDirectory(_T(".."));
		}
	} while (FindNextFile(SearchHandle,&FindData));
	
	FindClose(SearchHandle);

	return;
}

DWORD FileType (LPWIN32_FIND_DATA pFileData)
{
	BOOL IsDir;
	DWORD FType;
	FType = TYPE_FILE;
	IsDir = (pFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	if(IsDir)
		if(lstrcmp(pFileData->cFileName,_T(".")) == 0
			|| lstrcmp(pFileData->cFileName,_T(".."))==0)
			FType = TYPE_DOT;
		else FType = TYPE_DIR;
		return FType;
}