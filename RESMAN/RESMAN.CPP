#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>

typedef struct ttFileman
{
	char Name[18];
	struct ttFileman *next;
} tagFileman;

typedef tagFileman* lpFileman;

int   myItem = 0;			// 화일수
lpFileman Header = NULL;    // 화일 리스트
char  LSTpath[_MAX_PATH];   // 경로



//////////////////////////////////////////
//
void InsertFileinList(LPSTR filename)
{
	lpFileman makeone;

	makeone = new tagFileman;

	wsprintf(makeone->Name,"%s",filename);

	makeone->next = Header;
	Header = makeone;

	
	printf("%s\n",filename);
}



//////////////////////////////////////////
//
void DeleteFile(lpFileman koreta)
{
	lpFileman temp = Header;
	lpFileman makeone = Header;
	lpFileman delme;
	while(temp!=NULL)
	{
		if(temp == koreta)
		{
			delme = temp;
			if(Header == temp)
			{
				Header = Header->next;
			}
			else makeone->next =temp->next;
			delete delme;
			return;
		}
		makeone = temp;
		temp = temp->next;
	}
}


//////////////////////////////////////////
// Path 얻기
void GetPath(LPSTR source)
{
	char *tempstr = source;
	int i = strlen(tempstr);

	for(int loop = i; loop > 0; loop--)
	{
		if(tempstr[loop] == '\\')
		{
			memcpy(LSTpath,tempstr,loop);
			LSTpath[loop+1] = 0;
			break;
		}
	}
}



//////////////////////////////////////////
//
void ReadListFile(LPSTR listfile)
{
	FILE *fp;
	char Buffer[256];
	char strings[_MAX_FNAME + _MAX_EXT];

	myItem = 0;

	fp = fopen(listfile,"rt");

	if(fp == NULL)
	{
		printf("File not found.\n");
		exit(-1);
	}

	GetPath(listfile);							// 패쓰 얻기

	do
	{
		if(fgets(Buffer,256,fp) == NULL) break;
		if(Buffer[0]==10) continue;				// 아무것도 없으면 다시 읽기
        if (!Buffer) continue;
        if (!strcmp(Buffer,"//")) continue;
		sscanf(Buffer,"%s",&strings);
		InsertFileinList(strings);
		myItem++;
    } while(1);
    fclose(fp);
}


///////////////////////////////////////////////////////
//
void SaveResourcefile(LPSTR filename)
{
	lpFileman mtemp = Header;

	FILE *resfile,*listfile,*tempfile;
	char szFileName[_MAX_PATH];

	BYTE  *pointers;
	LONG filepos = 0;
	LONG filesz;

	printf("  # Output file : %s.LST, %s.ROM\n\n",filename,filename);

	wsprintf(szFileName,"%s.LST",filename);
	listfile = fopen(szFileName,"wb");			// file list 
	wsprintf(szFileName,"%s.ROM",filename);
	resfile = fopen(szFileName,"wb");			// file resource 

   

	fwrite(&myItem,4,1,listfile);
	
	while(mtemp != NULL)
	{
		tempfile = fopen(mtemp->Name,"rb");
		filesz = _filelength(_fileno(tempfile));
		printf("FILE READ : %s file size : %d\n",mtemp->Name,filesz);
		pointers = new BYTE[filesz];
		fread((void *)pointers,filesz,1,tempfile);

		// Put List file name & index
		fwrite((void *)mtemp->Name,18,1,listfile);
		fwrite(&filepos,4,1,listfile);
		filepos+=filesz;

		// Put Image file 
		fwrite((void *)pointers,filesz,1,resfile);

		fclose(tempfile);  // close file
		printf("FILE      : Writed...\n");
		mtemp = mtemp->next;
		delete pointers;
	}

	printf(" ** Total file size : %d\n",filepos);
	fclose(listfile);
	fclose(resfile);
}

void main(int argc,char *argv[])
{
  printf("==============================================================================\n");
  printf("  Resource manager\n");
  printf("  --------------------------------------------------------------------------  \n");	 

  if(argc == 1 || argc < 2)
  {
	  printf("RESMAN [list file name] [Resource file name]\n");
  } else 
  {
		printf("  # List source file : %s\n",argv[1]);
		ReadListFile(argv[1]);
		SaveResourcefile(argv[2]);
  }

  printf("==============================================================================\n");
}