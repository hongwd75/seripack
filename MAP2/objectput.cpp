#include"stdafx.h"
#include <stdio.h>
#include"resource.h"
#include"background.h"

/*
   ======================================================================================
   Output   : Text 형태
   --------------------
   1줄      : [적의 이름] [index]
   2줄      : 위치 [x] [y] >> 멥상의 위치를 나타낸다.
   3줄      : option 1, option 2

   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    option 1
	            0. 한번만 나온다.				Bit 1
				1. 계속해서 등장

				0. 화면밖에 있어도 움직인다.	Bit 2
				1. 화면에 등장하면 움직인다.

    option 2    뾰족 뾰족 튀어 나오는 것들하고, 왼쪽/오른쪽 구분 하는 적을 위해
	            [상위 2byte]                    [하위 2byte]
   ======================================================================================
*/



inmain		InObjectlist;
outmain		OutObjectlist;
extern mhsystem		wondae;
extern BG       *Scren;
extern int img_width,img_height;
extern HWND	 hwnd,Tilehwnd;
extern RGBstruct *ReadBMPfile(char *filename,int kk);
//////////////////////////////////////////////////////////
//		링크드 리스트 초기화 
//		처음에 한번만 불러줘..
void initlinklist(void)
{
	OutObjectlist.Header = NULL;
}

void GetPath2(LPSTR source)
{
	char *tempstr = source;
	int i = strlen(tempstr);

	for(int loop = i; loop > 0; loop--)
	{
		if(tempstr[loop] == '\\')
		{
			memcpy(InObjectlist.Path,tempstr,loop);
			InObjectlist.Path[loop+1] = 0;
			break;
		}
	}
}



void PutImage(int x,int y,int index)
{
	int addr = 0;
	if(index<0) return;

	if(x>639 || y > 479) return;

	int width = InObjectlist.data[index].width;
	int height = InObjectlist.data[index].height;

	RGBstruct	*mem = Scren->m_backimg;
	RGBstruct	*img = InObjectlist.data[index].image;

	for(int yy = 0; yy < height; yy++)
	{
		for(int xx = 0; xx < width; xx++)
		{
			if((x+xx < 0) || (y + yy < 0)) continue;
			if((x+xx >639) || (y + yy >479)) continue;
			addr = yy * width + xx;
			if(!(img[addr].R == 255 && img[addr].G == 0 && img[addr].B == 255))
			{
				int addr2 = (640 * (yy+y)) + (x + xx);
				mem[addr2].R = img[addr].R;
				mem[addr2].G = img[addr].G;
				mem[addr2].B = img[addr].B;
			}
		}
	}
}



int ArrayIndex(int index)
{
	for(int i = 0; i < InObjectlist.maxiumobject; i++)
	{
		if(InObjectlist.data[i].index == index) return i;
	}
	MessageBox(NULL,"널이다 널","File not found.",MB_OK | MB_ICONSTOP);
	return -1;
}



void ShowObject(void)
{

	LPlinkout Header = OutObjectlist.Header;
	while(Header)
	{
		PutImage(Header->x - (wondae.showx << 5),Header->y - (wondae.showy << 5),ArrayIndex(Header->index));
		Header = Header->next;
	}
}

///////////////////////////////////////////////////
// 
//
void InsertMon(LPSTR name,int index,int px,int py,int op2)
{
	LPlinkout temp;

	temp = (LPlinkout)malloc(sizeof(taglinkout));

	temp->x = px;
	temp->y = py;
	temp->option1 = 1 + 2;
	temp->option2 = op2;
	temp->clicked = 0;
	temp->index = index;
	sprintf(temp->name,"%s",name);

	if(OutObjectlist.Header == NULL)
	{
		OutObjectlist.Header = temp;
		temp->next = NULL;
	} else
	{
		temp->next = OutObjectlist.Header;
		OutObjectlist.Header = temp;
	}
}



///////////////////////////////////////////////////
// 
//
void WriteObject(LPSTR filename)
{
	FILE *fp;
	LPlinkout Header = OutObjectlist.Header;

	fp = fopen(filename,"wt");
	while(Header)
	{
		fprintf(fp,"%s %d %d %d %d %d\n",Header->name,Header->index,Header->x,Header->y,Header->option1,Header->option2);
		Header = Header->next;
	}
	fclose(fp);
	
}


void ReadObject(LPSTR filename)
{
	FILE *fp;
	char Buffer[256];
	char name[30];
	int px,py,op2,temp,index;

	initlinklist();
	fp = fopen(filename,"rt");
	if(fp == NULL) return;
	do
	{
		if(fgets(Buffer,256,fp) == NULL) break;
		sscanf(Buffer,"%s %d %d %d %d %d",&name,&index,&px,&py,&temp,&op2);
		InsertMon(name,index,px,py,op2);
	} while(1);
	fclose(fp);
}


///////////////////////////////////////////////////
// 
//
void ReadObjectList(LPSTR filename)
{
	FILE *fp;
	char Buffer[256];
	char filenn[20];

	InObjectlist.maxiumobject = 0;

	int loop = 0;
	SendDlgItemMessage(Tilehwnd, IDC_OBJECTCOMBO1,CB_RESETCONTENT,0,0);

	fp = fopen(filename,"rt");
	if(fp != NULL)
	{
		GetPath2(filename);
		do
		{
			if(fgets(Buffer,256,fp) == NULL) break;
	        if (!Buffer) continue;
		    if (!strcmp(Buffer,"//")) continue;
			// data struct
			// [object name] [object index] [object filename]
			sscanf(Buffer,"%s %d %s",&InObjectlist.data[InObjectlist.maxiumobject].name,
							  		 &InObjectlist.data[InObjectlist.maxiumobject].index,
									 &filenn);

			// *****************************
			char pername[256];
			sprintf(pername,"%s\\%s",InObjectlist.Path,filenn);
			InObjectlist.data[InObjectlist.maxiumobject].image = ReadBMPfile(pername,10);
			InObjectlist.data[InObjectlist.maxiumobject].width = img_width;
			InObjectlist.data[InObjectlist.maxiumobject].height = img_height;
			// ****************************** BMP 읽는다.

			SendDlgItemMessage(Tilehwnd, IDC_OBJECTCOMBO1,CB_INSERTSTRING,InObjectlist.maxiumobject,(LPARAM)InObjectlist.data[InObjectlist.maxiumobject].name);
			InObjectlist.maxiumobject++;
		}  while(1);
		fclose(fp);
		SendDlgItemMessage(Tilehwnd, IDC_OBJECTCOMBO1,CB_SETCURSEL,0,0);
		InObjectlist.selobject = 0;
	} else MessageBox(NULL,filename,"File not found.",MB_OK | MB_ICONSTOP);
}