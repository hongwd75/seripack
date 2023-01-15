// danceedit.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "background.h"
#include "file.h"
#include "resource.h"



#define		MOTIONMAX					256
#define		USER_GETINPUT 			 0x5FF1
#define     _DELAY_COMBO				  5

///////////////////////////////////////////////////////
#define    _UP							0
#define	   _DOWN						1
#define    _LEFT						2
#define    _RIGHT						3
#define    _HAND						4
#define    _FOOT						5
/////////////////////////////////////////////////////////


typedef struct {
	HWND	hwnd;
	HANDLE	hEvent;
	BOOL	bContinue;
} PARAMs, *PPARAMS;

typedef struct {
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	char szFileName[_MAX_PATH];
	ClImage *img;
} ListFileInfo;


typedef struct {
	char	szTitleName[_MAX_FNAME + _MAX_EXT];
	char	delaystring[6];
	char    position[8];
	int		ListFileIndex;
	int		delay;
	POINT	charpoint;
} MotionInfo;


struct tagDanceList {
	struct			tagDanceList *prev;		// 링크드용
	struct			tagDanceList *next;		// 링크드용
	int				MaxMotion;		  		// 모션수

	MotionInfo		MotionList[MOTIONMAX];	// 모션 데이타가 들어간다.
	char			MotionName[150];		// 모션 이름
	int             Button[9];				// 버튼데이타가 들어간다. [0] : 버튼수 [1....8] 데이타.
};


//////////////////////////////////////////////////////////////////////////////
typedef struct tagDanceList* LPDanceList;	// 포인터용 정의
typedef struct tagDanceList  mDanceList;	// 포인터용 정의
//////////////////////////////////////////////////////////////////////////////
extern WORD GetColorValue(int R,int G, int B);

ClImage		 *Logo;
BG			 *m_BK=NULL,*m_BK2;
HINSTANCE	  hInst;
HCURSOR		  m_cross;
HWND		  hwnd,modalhwnd,DirList;
ListFileInfo  Listfiles[255]; 
LPDanceList   DanceMotion = NULL;			// 동작들을 구분해서 넣어 놓은 곳.
LPDanceList	  NEditpos = NULL;				// 지금 편집하는 동작들 있은 포인터!
LPDanceList   EndHeader = NULL;				// 포인터 끝의 인간.
int           TitleMotionnum = 0;			// 모션의 수
int			  maxListfile=0;
int			  Filelists = 0;				// 이미지 화일수
BOOL		  autoreplay = FALSE;			// 자동 반복 플레이용
BOOL		  autostop = FALSE;				// 자동 반복땜에 생긴 스톱!
BOOL		  PlayFrame = FALSE;
BOOL		  ReadyFrameTick = FALSE;
HICON		  icons[7];

WORD		  mTransColor = GetColorValue(255,0,255);


/////////////////////////////////
//          재  성  의
//
void AddDanceData(LPSTR title);
void ReSetListComboBox(void);
void ReSetImageList(void);
void AllDeleteDanceData(void);
void ReSetMotionList(void);
void MoveTrackBar(int);
void InitTrackBar(int MaxiumNum);

void ErrorLog(char *filename,char *msg)
{
	FILE *fp;

	fp =fopen(filename,"at");
	fprintf(fp,msg);
	fprintf(fp,"\n");
	fclose(fp);
}

/////////////////////////////////////////////////////////////////////////
// 리스트화일 저장
//
void WriteToDisk(LPSTR filename)
{
	FILE *fp;
	int loop;
	int length;

	char title[255];
	LPDanceList  thiss = DanceMotion;
	fp = fopen(filename,"wb");

	fwrite(&Filelists,4,1,fp);		//BMP 화일수 [4 BYTE]

	for(loop = 0; loop < Filelists; loop ++)
	{
		length = strlen(Listfiles[loop].szFileName);
		fwrite(&length,4,1,fp);
		fwrite(&Listfiles[loop].szFileName,length,1,fp);

		length = strlen(Listfiles[loop].szTitleName);
		fwrite(&length,4,1,fp);
		fwrite(&Listfiles[loop].szTitleName,length,1,fp);


	}  // end of BMP file list.

	while(thiss !=NULL)
	{
		length = strlen(thiss->MotionName);
		fwrite(&length,4,1,fp);
		fwrite(&thiss->MotionName,length,1,fp);
		fwrite(&thiss->MaxMotion,4,1,fp);
		fwrite(&thiss->Button,4,9,fp);
		for(loop=0;loop<thiss->MaxMotion;loop++)
		{
			fwrite(&thiss->MotionList[loop].ListFileIndex,4,1,fp);
			fwrite(&thiss->MotionList[loop].delay,4,1,fp);
			fwrite(&thiss->MotionList[loop].charpoint, sizeof(POINT),1,fp);
		}
		thiss=thiss->next;
	} // 저장종료
	length = 1;
	fwrite(&length,4,1,fp);
	title[0]=1;
	fwrite(&title[0],1,1,fp);
	fclose(fp);
}



/////////////////////////////////////////////////////////////////////////
// 리스트화일 읽기
//
void ReadToDisk(LPSTR filename)
{
	FILE *fp;
	
	int loop;
	int length;
	char title[250];
	LPDanceList  thiss = DanceMotion;



	fp = fopen(filename,"rb");

	AllDeleteDanceData();						// 모든 데이타 지워주~~
	
	fread(&Filelists,4,1,fp);					//BMP 화일수 [4 BYTE]


	for(loop = 0; loop < Filelists; loop ++)
	{
		fread(&length,4,1,fp);
		fread((void *)Listfiles[loop].szFileName,length,1,fp);

		fread(&length,4,1,fp);
		fread((void *)Listfiles[loop].szTitleName,length,1,fp);

	}  // end of BMP file list.

	ReSetImageList();

	while(1)
	{
		int news;
		fread(&length,4,1,fp);
		fread((void *)title,length,1,fp);
		title[length]=0;
		news = strlen(title);

		if(title[0] != 1)	
		{
			AddDanceData(title);

			fread(&EndHeader->MaxMotion,4,1,fp);
			fread(&EndHeader->Button,4,9,fp);
			for(loop=0;loop<EndHeader->MaxMotion;loop++)
			{
				fread(&EndHeader->MotionList[loop].ListFileIndex,4,1,fp);
				fread(&EndHeader->MotionList[loop].delay,4,1,fp);
				fread(&EndHeader->MotionList[loop].charpoint, sizeof(POINT),1,fp);

				_itoa( EndHeader->MotionList[loop].delay, EndHeader->MotionList[loop].delaystring, 10 );
				wsprintf(EndHeader->MotionList[loop].position,"%d,%d",EndHeader->MotionList[loop].charpoint.x,EndHeader->MotionList[loop].charpoint.y);
				strcpy(EndHeader->MotionList[loop].szTitleName,Listfiles[EndHeader->MotionList[loop].ListFileIndex].szTitleName);
			}
		} else break;
	} // 로드종료

	fclose(fp);

	NEditpos = DanceMotion;
	ReSetListComboBox();
	ReSetMotionList();
}



/////////////////////////////////////////////////////////////////////////
// 땐쓰~ 모션, 화일들을 모두 메모리에서 지운다.(화일 오픈시 쓰이쥐)
//
void AllDeleteDanceData(void)
{
	LPDanceList temp;
	LPDanceList header = DanceMotion;

	NEditpos = NULL;
	Filelists = 0;
	while(header != NULL)
	{
		temp=header;
		header = header->next;
		delete temp;
	}
	DanceMotion= NULL;

}
/////////////////////////////////////////////////////////////////////////
// 땐쓰~ 모션 더하기.
//
void AddDanceData(LPSTR title)
{
	LPDanceList temp,temp2=DanceMotion;
	LPDanceList header = DanceMotion;


	temp = (LPDanceList)malloc (sizeof(mDanceList));

	// 초기화
	TitleMotionnum++;
	temp->next = NULL;
	temp->MaxMotion = 0;
	temp->Button[0] = 0;
	for(int hhhh=1;hhhh<9;hhhh++)temp->Button[hhhh]=6;
	for(int j=0;j<MOTIONMAX;j++) temp->MotionList[j].delay = 1;

	EndHeader = temp;								// 끝 포인터를 일기위해서

	//wsprintf(temp->MotionName,"%s",title);		// 이름 입력
	strcpy(temp->MotionName,title);

	if(header == NULL) {
		DanceMotion = temp;
//		temp->prev = NULL;
		return;
	} else 
	{
		while(header != NULL)
		{
			temp2= header;
			header = header->next;
		}
		temp2->next = temp;
		temp->prev = temp2;
	}
}



/////////////////////////////////////////////////////////////////////////
// 땐쓰~ 모션 삭제.
//
void DelDanceData(LPDanceList Deled)
{
	LPDanceList temp,temp2;
	LPDanceList header = Deled;
	TitleMotionnum--;
	if(Deled == DanceMotion)
	{
		DanceMotion = Deled->next;
		free (header);
		return;
	}
	temp = Deled->prev;
	temp2 = Deled->next;
	temp->next = temp2;
	if(Deled->next == NULL)
	{
		temp->next = NULL;
	} else
	{
		temp2->prev = temp;
	}
	free (header);
}


////////////////////////////////////////////////////////////////////////
// 땐쓰~ 모션 콤보 박스 재 설정
//
void ReSetListComboBox(void)
{
	int loop = 0;
	LPDanceList header = DanceMotion;
	SendDlgItemMessage(modalhwnd, IDC_COMBO3,CB_RESETCONTENT,0,0);
	while(header != NULL)
	{
		SendDlgItemMessage(modalhwnd, IDC_COMBO3,CB_INSERTSTRING,loop++,(LPARAM)header->MotionName);
		header = header->next;
	}
}



/////////////////////////////////////////////////////////////////////////
//  모션 리스트 추가.
//
void AddMotionList(int nCur, int delay)
{
	LV_ITEM item1;
	item1.mask = LVIF_TEXT;
	item1.iItem = NEditpos->MaxMotion++;

	item1.iSubItem = 0;
	NEditpos->MotionList[item1.iItem].ListFileIndex = nCur;		// 리스트 화일 Index
	strcpy(NEditpos->MotionList[item1.iItem].szTitleName,Listfiles[nCur].szTitleName);
	item1.pszText = (LPTSTR)NEditpos->MotionList[item1.iItem].szTitleName;
	ListView_InsertItem(GetDlgItem(modalhwnd,IDC_LIST2),&item1);
    
	item1.iSubItem = 1;
	NEditpos->MotionList[item1.iItem].delay= delay;
	_itoa( NEditpos->MotionList[item1.iItem].delay, NEditpos->MotionList[item1.iItem].delaystring, 10 );
	item1.pszText = (LPTSTR)NEditpos->MotionList[item1.iItem].delaystring;
	ListView_SetItem(GetDlgItem(modalhwnd,IDC_LIST2),&item1);
	item1.iSubItem = 2;
	NEditpos->MotionList[item1.iItem].charpoint.x = 0;
	NEditpos->MotionList[item1.iItem].charpoint.y = 0;
	wsprintf(NEditpos->MotionList[item1.iItem].position,"%d,%d",
		     NEditpos->MotionList[item1.iItem].charpoint.x,
			 NEditpos->MotionList[item1.iItem].charpoint.y);
	item1.pszText = (LPTSTR)NEditpos->MotionList[item1.iItem].position;
	ListView_SetItem(GetDlgItem(modalhwnd,IDC_LIST2),&item1);
}



void DrawICONHDC(void)
{
	HDC hDC = GetDC(modalhwnd);
	for(int i=1;i<9;i++)
		//DrawIconEx(hDC,476+(i-1)*38,596,icons[NEditpos->Button[i]],16,16,0,NULL,DI_NORMAL);
		DrawIcon(hDC,476+(i-1)*36,596,icons[NEditpos->Button[i]]);
	InvalidateRect(modalhwnd,NULL,FALSE);
}
/////////////////////////////////////////////////////////////////////////
//	모션 리스트 재설정
//
void ReSetMotionList(void)
{
	LV_ITEM item1;
	item1.mask = LVIF_TEXT;

    ListView_DeleteAllItems(GetDlgItem(modalhwnd,IDC_LIST2));
	if (NEditpos->MaxMotion>0)
	{
		for(int i = 0; i < NEditpos->MaxMotion; i++)
		{
			item1.iItem = i;
			item1.iSubItem = 0;
			item1.pszText = (LPTSTR)NEditpos->MotionList[item1.iItem].szTitleName;
			ListView_InsertItem(GetDlgItem(modalhwnd,IDC_LIST2),&item1);           // 추가
	
			item1.iSubItem = 1;
			item1.pszText = (LPTSTR)NEditpos->MotionList[item1.iItem].delaystring;
			ListView_SetItem(GetDlgItem(modalhwnd,IDC_LIST2),&item1);
		}
	}
	DrawICONHDC();
}



/////////////////////////////////////////////////////////////////////////
// 모션 리스트에서 하나 모션 삭제
//
void DeleteMotionList(int num)
{
	for(int i=num;i<NEditpos->MaxMotion - 1;i++)
	{
		NEditpos->MotionList[i].charpoint.x = NEditpos->MotionList[i+1].charpoint.x;
		NEditpos->MotionList[i].charpoint.y = NEditpos->MotionList[i+1].charpoint.y;
		NEditpos->MotionList[i].delay = NEditpos->MotionList[i+1].delay;
		NEditpos->MotionList[i].ListFileIndex = NEditpos->MotionList[i+1].ListFileIndex;

		strcpy(NEditpos->MotionList[i].position,NEditpos->MotionList[i+1].position);
		strcpy(NEditpos->MotionList[i].delaystring,NEditpos->MotionList[i+1].delaystring);
		strcpy(NEditpos->MotionList[i].szTitleName,NEditpos->MotionList[i+1].szTitleName);
	}
	NEditpos->MaxMotion--;
	ReSetImageList();
	ReSetMotionList();
}



/////////////////////////////////////////////////////////////////////////
//	이미지 리스트 재설정
//
void ReSetImageList(void)
{
	LV_ITEM item1;
	item1.mask = LVIF_TEXT;

    ListView_DeleteAllItems(GetDlgItem(modalhwnd,IDC_LIST1));
	for(int loop=0;loop<Filelists;loop++)
	{
		item1.iItem =loop;
		item1.iSubItem = 0;
		item1.pszText = (LPTSTR)Listfiles[item1.iItem].szTitleName;
		ListView_InsertItem(GetDlgItem(modalhwnd,IDC_LIST1),&item1);

		item1.iSubItem = 1;
		item1.pszText = (LPTSTR)Listfiles[item1.iItem].szFileName;
		ListView_SetItem(GetDlgItem(modalhwnd,IDC_LIST1),&item1);
		if(Listfiles[item1.iItem].img->LoadBMP(Listfiles[item1.iItem].szFileName) == FALSE)
		{
		}
	}

}



/////////////////////////////////////////////////////////////////////////
// 프레임 플레이 loop
//
int playloop=0;

BOOL FRAMEPLAYLOOP(void)
{

	static int delays=0;
	if(autostop)
	{
		delays = 0;
		playloop = 0;
		autostop = FALSE;
		InitTrackBar(NEditpos->MaxMotion);
		SendMessage(modalhwnd,WM_COMMAND,IDC_STOP,0);
		return FALSE;
	}
	if(playloop<NEditpos->MaxMotion)
	{
		m_BK->PutImageInWorld(NEditpos->MotionList[playloop].charpoint.x,NEditpos->MotionList[playloop].charpoint.y,
			Listfiles[NEditpos->MotionList[playloop].ListFileIndex].img->width,
			Listfiles[NEditpos->MotionList[playloop].ListFileIndex].img->height,
			Listfiles[NEditpos->MotionList[playloop].ListFileIndex].img->image);
		delays++;
		if(delays >= NEditpos->MotionList[playloop].delay)
		{
			playloop++;	
			MoveTrackBar(playloop);
			delays=0;
		}

	} else {
		delays =0;
		playloop = 0;
		InitTrackBar(NEditpos->MaxMotion);
		
		if(!autoreplay) 
		{
			SendMessage(modalhwnd,WM_COMMAND,IDC_STOP,0);
			return FALSE;
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//
//
void Init_SYSTEM(void)
{
	TitleMotionnum = 0;
	m_BK = new BG(15,25,512,450,FALSE,modalhwnd,hInst);
	m_BK2 = new BG(780,500,128,130,FALSE,modalhwnd,hInst);
	m_BK->MakeVirMemory(512,450);
	m_BK2->MakeVirMemory(128,130);
	m_BK2->m_resizewindow = TRUE;
	Logo = new ClImage();
	Logo->LoadBMP("logo.bmp");
	m_BK->PutImage(0,-50,Logo->width,Logo->height,Logo->image);
	for(int i=0;i<MOTIONMAX;i++) Listfiles[i].img = new ClImage();

	
//	for(int j=0;j<513;j++) MotionList[j].delay = 1;
	m_cross = LoadCursor(hInst,"GetCurs");
//	DanceMotion->next = NULL;
	icons[_UP]    = LoadIcon(hInst,MAKEINTRESOURCE(IDI_UP));
	icons[_DOWN]  = LoadIcon(hInst,MAKEINTRESOURCE(IDI_DOWN));
	icons[_LEFT]  = LoadIcon(hInst,MAKEINTRESOURCE(IDI_LEFT));
	icons[_RIGHT] = LoadIcon(hInst,MAKEINTRESOURCE(IDI_RIGHT));
	icons[_HAND]  = LoadIcon(hInst,MAKEINTRESOURCE(IDI_HAND2));
	icons[_FOOT]  = LoadIcon(hInst,MAKEINTRESOURCE(IDI_FOOT));
	icons[6]      = LoadIcon(hInst,MAKEINTRESOURCE(IDI_NOT));
}


BOOL CALLBACK DigProc(HWND,UINT,WPARAM,LPARAM);
HIMAGELIST CreateMultiDrag(HWND hList,int nItem,POINT* pHotPoint);
long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT	CALLBACK InputProc(HWND hdlg,UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	static char szAppName[] = "Dance Motion Edit";
	MSG			 msg;
	hInst = hInstance;
	WNDCLASSEX wndclass;
	
	ZeroMemory(&wndclass,sizeof(wndclass));
	if(!hPrevInstance) 
	{
		wndclass.cbSize			= sizeof(wndclass);
		wndclass.style			= CS_HREDRAW | CS_VREDRAW ;
		wndclass.lpfnWndProc	= (WNDPROC)WindowProc;
		wndclass.hInstance		= hInst; 
		wndclass.hCursor		= LoadCursor(NULL,IDC_ARROW);
		wndclass.hIcon			= LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
		wndclass.hbrBackground	= (HBRUSH)(COLOR_MENU+1);
		wndclass.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1); 
		wndclass.lpszClassName	= szAppName;
		wndclass.hIconSm		= LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));

		RegisterClassEx(&wndclass);
		hwnd = CreateWindow(szAppName,szAppName,WS_BORDER | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,0,0,60,60,NULL,NULL,hInst,NULL);
	}

	modalhwnd = CreateDialog(hInst,"IDD_DIG",hwnd,DigProc);
    ShowWindow( hwnd, nCmdShow);
    UpdateWindow(hwnd); 

	Init_SYSTEM();					// 시스템 초기화.

   while(1) 
   { 
	   if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )  
		{
			if( !GetMessage( &msg, NULL, 0, 0 ) ) 
			{
				 return msg.wParam;
			}
			if(modalhwnd == 0 || !IsDialogMessage(modalhwnd,&msg))
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
   }
   return msg.wParam;
}



long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_CREATE:
				InitCommonControls();
			return 0;

		case WM_COMMAND:
			return 0;
		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_F5:
					PlayFrame=TRUE;
				break;
			}
			return 0;

		case WM_PAINT:
			{
                /* 줄 표시 */
			}
		break; 

		case WM_CLOSE:
			{
				int askx = MessageBox(hWnd,"Do you want to close PROGRAM ?","CLOSE",MB_YESNO|MB_ICONQUESTION);
				if(askx == IDYES) 
				{
					PostMessage(modalhwnd , WM_CLOSE, 0, 0);
					modalhwnd = 0;
					DestroyWindow(hWnd);
				}
			}
			return 0;

        case WM_DESTROY:
             PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


void ResetDelay(int item,int newdelay)
{
	LV_ITEM item1;
	item1.mask = LVIF_TEXT;
	item1.iItem = item;
	item1.iSubItem = 1;
	NEditpos->MotionList[item1.iItem].delay = newdelay;
	_itoa( NEditpos->MotionList[item1.iItem].delay, NEditpos->MotionList[item1.iItem].delaystring, 10 );
	item1.pszText = (LPTSTR)NEditpos->MotionList[item1.iItem].delaystring;
	ListView_SetItem(GetDlgItem(modalhwnd,IDC_LIST2),&item1);

}


/*
void ConvertCMP2(int width,int height,WORD *Data,FILE *fp)
{
	WORD *tempbuffer;
	int  *heights;
	WORD *Dest;

	
	int srccount = 0;			// Data 주소 카운터 
	int memcount = 0;			// Dest 주소 카운터
	int bufferpos = 0;			// styles가 들어갈 주소 카운터
	int transcount;
	int wordpos;                // 1픽셀전송용
	int wordsize;
	int styles;					// 몇개의 
	int hcount = 0;
	int mswitch;

	int tempcount;
	int csize;

	tempbuffer = new WORD[width * 2];
	heights = new int[height];
	Dest    = new WORD[width * height * 2];

	while(hcount < height)
	{
		styles = 0;
		heights[hcount] = memcount;
		bufferpos = memcount++;
		mswitch = 0;
		transcount = 0;
		for(int i=0; i < width; i++)
		{
			switch(mswitch)
			{
				case 0:
					if(Data[srccount+i] == mTransColor) transcount++;
					else 
					{
						i--;
						mswitch = 1;
						Dest[memcount++] = transcount;
						transcount=0;
						tempcount=0;
						csize = 0;
					}
					break;
				case 1:
					if(Data[srccount+i] != mTransColor)
					{
						tempbuffer[tempcount++] = Data[srccount+i];
					} else 
					{
						i--;
						mswitch = 2;
					}
					break;
				case 2:
					Dest[memcount++] = tempcount;
					for(int j=0;j<tempcount;j++)
					{
						Dest[memcount++] = tempbuffer[j];
					}
					if(i<width-1)
					{
						mswitch = 0;
						i--;
					}
					styles++;
					break;
			}
		}

		if(mswitch == 0)
		{
			Dest[memcount++] = transcount;
			Dest[memcount++] = 0;
			styles++;
		}

		if(mswitch == 1)
		{
			Dest[memcount++] = tempcount;
			if(tempcount>0)
			{
				for(int j=0;j<tempcount;j++)
				{
					Dest[memcount++] = tempbuffer[j];
				}
			}
			styles++;
		}
		Dest[bufferpos] = styles;
		hcount++;
		srccount+=width;
	}
   // 가로, 세로, 세로데이타주소값, 데이타 변수가 있어야 한다.

//	FILE   *fp;
	char HEADER[4];

//	fp = fopen(filename,"wb");
	
	strcpy(HEADER,"SP1");
	fwrite(&HEADER,3,1,fp);
	fwrite(&width,4,1,fp);
	fwrite(&height,4,1,fp);
	fwrite(heights,height,4,fp);
	fwrite(&memcount,4,1,fp);
	fwrite(Dest,memcount*2,1,fp);
//	fclose(fp);

	delete tempbuffer;
	delete heights;
	delete Dest;
}
*/
#define BLANKTYPE			0				// 투명컬러일때
#define PIXELTYPE			1				// 일반컬러일때

void ConvertCMP2(int width,int height,WORD *Data,FILE *file)
{
	WORD *tempbuffer;
	int  *heights;
	WORD *Dest;

	
	int stylecount = 0;				// [투명색][컬러수] 라는 스타일이 몇개나 들어있나.
	int styleBufferAddr = 0;		// styles가 들어갈 주소 카운터
	int ColorSizeBufferAddr = 0;	

	int srccount = 0;				// Data 주소 카운터 
	int memcount = 0;				// Dest 주소 카운터
	int count = 0;					// 카운터용
	int styles;

	tempbuffer = new WORD[width * 3];
	heights = new int[height];
	Dest    = new WORD[width * height * 3];

	for(int y = 0; y < height; y++)
	{
		styles = BLANKTYPE;				// 투명으로 세팅
		count = 0;						// 투명색은 0개이다.

		heights[y] = memcount;			// 세로 시작 번지

		stylecount	= 0;				// 스타일 수 카운터 0으로 초기화
		styleBufferAddr = memcount++;	// 스타일수가 들어가 버퍼

		for(int x = 0; x < width; x++)
		{
			if(Data[srccount] == mTransColor)
			{
				if(styles == BLANKTYPE) count++;	// 투명색의 연속이면 더해준다
					else							// 투명색이 아님
					{
						Dest[ColorSizeBufferAddr] = count;
						// 투명으로 초기화
						count = 1;
						styles = BLANKTYPE;
					}
			} else 
			{
				if(styles == BLANKTYPE)
				{
					stylecount++;						// 스타일 카운터 증가.
					Dest[memcount++] = count;			// 투명색 넣고 
					ColorSizeBufferAddr = memcount;		// 버퍼 어드레스 세팅
					memcount++;							// 타켓 버퍼 어스레스 증가
					count = 1;
					styles = PIXELTYPE;
				} else count++;
				Dest[memcount++] = Data[srccount];		// 컬러값 넣고 메모리 증가
			}
			srccount++;
		}  // End of X
		
		if(styles == BLANKTYPE) 
		{
			stylecount++;
			Dest[memcount++] = count;
			Dest[memcount++] = 0;
		} else 
		{
			Dest[ColorSizeBufferAddr] = count;
		}

		Dest[styleBufferAddr] = stylecount;
	}  // End of y

	FILE   *fp = file;
	char HEADER[4];

	//fp = fopen(filename,"wb");
	
	strcpy(HEADER,"SP1");
	fwrite(&HEADER,3,1,fp);
	fwrite(&width,4,1,fp);
	fwrite(&height,4,1,fp);
	fwrite(heights,height,4,fp);
	fwrite(&memcount,4,1,fp);
	fwrite(Dest,memcount*2,1,fp);
//	fclose(fp);
	delete tempbuffer;
	delete heights;
	delete Dest;
}

/////////////////////////////////////////////////////////////////////////
//
//
BOOL CALLBACK DigProc(HWND hdlg,UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static char szFileName[_MAX_PATH];
	static char szTitleName[_MAX_FNAME + _MAX_EXT];


	static int		nItem;
	static int		nDragItem = -1;
	static POINT	pt,ptHotSpot;

	static HIMAGELIST	hDragList = NULL;
	static HIMAGELIST	hCursors = NULL;

	static L2Click	= -1;
	static BOOL	IsDrag = FALSE;
	static BOOL InList2 = FALSE;
	


	switch(iMsg)
	{
			case WM_INITDIALOG:
			{
 				int loop;
				char ffz[50];

				ICONINFO info;

				PopFileInit(hdlg);
				for(loop=0;loop<280;loop++)
				{
					wsprintf(ffz,"%d",loop+1);
					SendDlgItemMessage(hdlg, IDC_COMBO1,CB_INSERTSTRING,loop,(LPARAM)ffz);
				}
				SendDlgItemMessage(hdlg, IDC_COMBO1,CB_SETCURSEL,0,0);
				SendDlgItemMessage(hdlg, IDC_LOOPPLAY,BM_SETCHECK,0,0);
				LV_COLUMN col;
				col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
				col.fmt = LVCFMT_LEFT;

				col.cx =120;
				col.pszText = "File";
				col.iSubItem = 0;
				SendDlgItemMessage(hdlg, IDC_LIST1,LVM_INSERTCOLUMN,0,(LPARAM)&col);
				
				col.cx =250;
				col.pszText = "Path";
				col.iSubItem = 1;
				SendDlgItemMessage(hdlg, IDC_LIST1,LVM_INSERTCOLUMN,1,(LPARAM)&col);

				col.cx =140;
				col.pszText = "File";
				col.iSubItem = 0;
				SendDlgItemMessage(hdlg, IDC_LIST2,LVM_INSERTCOLUMN,0,(LPARAM)&col);
				col.cx =45;
				col.pszText = "지연";
				col.iSubItem = 1;
				SendDlgItemMessage(hdlg, IDC_LIST2,LVM_INSERTCOLUMN,1,(LPARAM)&col);
				col.cx =90;
				col.pszText = "위치";
				col.iSubItem = 2;
				SendDlgItemMessage(hdlg, IDC_LIST2,LVM_INSERTCOLUMN,2,(LPARAM)&col);
			
				// 핫 스팟 정보 얻기.
				GetIconInfo(LoadCursor(NULL,IDC_ARROW),&info);
				ptHotSpot.x = info.xHotspot;
				ptHotSpot.y = info.yHotspot;
				DeleteObject(info.hbmMask);
				DeleteObject(info.hbmColor);
			}
		break;

		case WM_TIMER:
			{
				FRAMEPLAYLOOP();
			}
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_MAKERESOURCE:
					{
						FILE *fp;
						WORD *image;
						char header[4];
						char mess[40];
						int ii,xx,yy,mmax;
						char FNx[_MAX_PATH];
						
						POINT ss;
						LPDanceList   templist = DanceMotion;

						szFileName[0] = NULL;
						szTitleName[0] = NULL;
						if(PopRESFileSaveDlg(hdlg,szFileName,szTitleName))
						{
							wsprintf(FNx,"%s",szFileName);
							fp = fopen(FNx,"wb");
							strcpy(header,"CHR");
							fwrite(&header,3,1,fp);				// HEADER
							fwrite(&Filelists,4,1,fp);			// Image files num 
							for(ii=0;ii<Filelists;ii++)
							{
								xx    = Listfiles[ii].img->width;
								yy    = Listfiles[ii].img->height;
								image = Listfiles[ii].img->Get16BitImage();
								ConvertCMP2(xx,yy,image,fp);
//								strcpy(header,"NO1");			
//								fwrite(&header,3,1,fp);			// Normal image format
//								fwrite(&xx,4,1,fp);				// width size
//								fwrite(&yy,4,1,fp);				// height size
//								fwrite(image,xx*yy,2,fp);		// image data
								delete image;

							}
							fwrite(&TitleMotionnum,4,1,fp);		// 동작타이틀 수
							wsprintf(mess,"동작 타이틀 수 : %d",TitleMotionnum);
							MessageBox(NULL,mess,MB_OK, NULL);
							while(templist != NULL)
							{
								mmax = templist->MaxMotion;
								fwrite(&mmax,4,1,fp);
								//fwrite(&templist->Button,4,9,fp);
								wsprintf(mess,"pass 5 :size [%d]",mmax);
								for(ii=0;ii<mmax;ii++)
								{

									xx = templist->MotionList[ii].ListFileIndex;		// image number
									yy = templist->MotionList[ii].delay;				// delay time
									ss = templist->MotionList[ii].charpoint;			// world position
									fwrite(&xx,4,1,fp);
									fwrite(&yy,4,1,fp);
									fwrite(&ss,sizeof(POINT),1,fp);
								}
								templist = templist->next;
							}
							fclose(fp);
						}
					}
				return TRUE;

				case IDC_BTUP:
					if(NEditpos != NULL)
					{
						if(NEditpos->Button[0] < 8)
						{
							NEditpos->Button[0]++;
							NEditpos->Button[NEditpos->Button[0]] = _UP;
						}
						DrawICONHDC();
					}
				return TRUE;
				case IDC_BTDOWN:
					if(NEditpos != NULL)
					{
						if(NEditpos->Button[0] < 8)
						{
							NEditpos->Button[0]++;
							NEditpos->Button[NEditpos->Button[0]] = _DOWN;
						}
						DrawICONHDC();
					}
				return TRUE;
				case IDC_BTLEFT:
					if(NEditpos != NULL)
					{
						if(NEditpos->Button[0] < 8)
						{
							NEditpos->Button[0]++;
							NEditpos->Button[NEditpos->Button[0]] = _LEFT;
						}
						DrawICONHDC();
					}
				return TRUE;
				case IDC_BTRIGHT:
					if(NEditpos != NULL)
					{
						if(NEditpos->Button[0] < 8)
						{
							NEditpos->Button[0]++;
							NEditpos->Button[NEditpos->Button[0]] = _RIGHT;
						}
						DrawICONHDC();
					}
				return TRUE;

				case IDC_BTHAND:
					if(NEditpos != NULL)
					{
						if(NEditpos->Button[0] < 8)
						{
							NEditpos->Button[0]++;
							NEditpos->Button[NEditpos->Button[0]] = _HAND;
						}
						DrawICONHDC();
					}
				return TRUE;
				case IDC_BTFOOT:
					if(NEditpos != NULL)
					{
						if(NEditpos->Button[0] < 8)
						{
							NEditpos->Button[0]++;
							NEditpos->Button[NEditpos->Button[0]] = _FOOT;
						}
						DrawICONHDC();
					}
				return TRUE;
				case IDC_LSTDEL:
					if(NEditpos != NULL)
					{
						if(NEditpos->Button[0] > 0)
						{
							NEditpos->Button[NEditpos->Button[0]] = 6;
							NEditpos->Button[0]--;
						}
						DrawICONHDC();
					}
				return TRUE;


				case IDC_NEWMN:
					AddDanceData("기본동작R");
					AddDanceData("기본동작L");
					AddDanceData("걷는다R");
					AddDanceData("걷는다L");
					AddDanceData("뛴다.R");
					AddDanceData("뛴다.L");
					AddDanceData("위로점프R");
					AddDanceData("위로점프L");
					AddDanceData("공중공격R");
					AddDanceData("공중공격L");
					AddDanceData("공격1R");
					AddDanceData("공격1L");
					AddDanceData("공격2R");
					AddDanceData("공격2L");
					AddDanceData("점프하강R");
					AddDanceData("점프하강L");
					AddDanceData("아프다(지상)R");
					AddDanceData("아프다(지상)L");
					AddDanceData("아프다(공중)R");
					AddDanceData("아프다(공중)L");
					AddDanceData("죽었을때");

					SendMessage(modalhwnd,WM_COMMAND,USER_GETINPUT,0);
					return TRUE;

				case IDC_LOOPPLAY:
						if(autoreplay) SendDlgItemMessage(hdlg, IDC_LOOPPLAY,BM_SETCHECK,0,0);
						else SendDlgItemMessage(hdlg, IDC_LOOPPLAY,BM_SETCHECK,1,0);
						autoreplay = 1 - autoreplay;
						
				return TRUE;

				case IDC_PLAY:
						if(NEditpos != NULL)
						{
							SetTimer(hdlg,1,70,NULL);
							PlayFrame=TRUE;
							InitTrackBar(NEditpos->MaxMotion);
						}
				return TRUE;

				case IDC_STOP:
					if(PlayFrame) {
						//autostop = TRUE;
						KillTimer(hdlg,1);
					}
				return TRUE;

				case IDC_COMBO1:
					if(HIWORD(wParam) == CBN_SELCHANGE && NEditpos != NULL && L2Click > -1 )
					{
						int ImageTypeSel = SendDlgItemMessage(hdlg, IDC_COMBO1,CB_GETCURSEL,0,0);
						ResetDelay(L2Click,ImageTypeSel+1);
					}
				return TRUE;

				case IDC_COMBO3:
					if(HIWORD(wParam) == CBN_SELCHANGE) 
					{
						int loop;
						int ImageTypeSel = SendDlgItemMessage(hdlg, IDC_COMBO3,CB_GETCURSEL,0,0);
						LPDanceList header = DanceMotion;
						
						for(loop=0;loop<ImageTypeSel;loop++)
						{
							header = header->next;
						}
						NEditpos = header;
						ReSetMotionList();	// 리스트 박스 재설정.
						L2Click = -1;
					}
				return TRUE;

				case IDC_REPOSITION:
						if(NEditpos != NULL && L2Click > -1)
						{
							m_BK->m_ViewCross = TRUE;
							m_BK->m_ImageCurs = TRUE;
							m_BK->Addpoint.x = NEditpos->MotionList[L2Click].charpoint.x;
							m_BK->Addpoint.y = NEditpos->MotionList[L2Click].charpoint.y;
							m_BK->m_ImageSize.x = Listfiles[NEditpos->MotionList[L2Click].ListFileIndex].img->width;
							m_BK->m_ImageSize.y = Listfiles[NEditpos->MotionList[L2Click].ListFileIndex].img->height;
						}
				return TRUE;

				case USER_GETPOS:
						NEditpos->MotionList[L2Click].charpoint.x = m_BK->Addpoint.x;
						NEditpos->MotionList[L2Click].charpoint.y = m_BK->Addpoint.y;
						m_BK->PutImageInWorld(NEditpos->MotionList[L2Click].charpoint.x,NEditpos->MotionList[L2Click].charpoint.y,
							Listfiles[NEditpos->MotionList[L2Click].ListFileIndex].img->width,
							Listfiles[NEditpos->MotionList[L2Click].ListFileIndex].img->height,
							Listfiles[NEditpos->MotionList[L2Click].ListFileIndex].img->image);
				return TRUE;

				case USER_GETINPUT:
					{
						int loop = 0;
						LPDanceList header = DanceMotion;
						SendDlgItemMessage(hdlg, IDC_COMBO3,CB_RESETCONTENT,0,0);
						while(header != NULL)
						{
							SendDlgItemMessage(hdlg, IDC_COMBO3,CB_INSERTSTRING,loop++,(LPARAM)header->MotionName);
							header = header->next;
						}
						SendDlgItemMessage(hdlg, IDC_COMBO3,CB_SETCURSEL,loop-1,0);
					}
				return TRUE;


				case IDC_MADD:			// 데이타 이름 입력 받는 곳.
					{
						DialogBox(hInst,"IDD_INPUT",hdlg,(DLGPROC)InputProc);
					}
					return TRUE;

				case IDC_MDEL:
					{
						if(NEditpos != NULL)
						{
							DelDanceData(NEditpos);
							SendMessage(hdlg,WM_COMMAND,USER_GETINPUT,0);
							L2Click = -1;
							NEditpos = NULL;
						}
					}
					return TRUE;

				case IDC_FDEL:
					{
						if(L2Click > -1) DeleteMotionList(L2Click);
						L2Click = -1;
					}	
					return TRUE;

				case IDC_MLOAD:
					if(PopLISTFileOpenDlg(hdlg,szFileName,szTitleName))
					{
						ReadToDisk(szFileName);
					}
					return TRUE;

				case IDC_MSAVE:
					if(PopLISTFileSaveDlg(hdlg,szFileName,szTitleName))
					{
						WriteToDisk(szFileName);
					}
					return TRUE;

				case IDC_ADDFILE :
					if(PopFileOpenDlg(hdlg,szFileName,szTitleName))
					{
						LV_ITEM item1;
						item1.mask = LVIF_TEXT;
						item1.iItem = Filelists++;
						item1.iSubItem = 0;
						wsprintf(Listfiles[item1.iItem].szTitleName,"%s",szTitleName);
						item1.pszText = (LPTSTR)Listfiles[item1.iItem].szTitleName;
						ListView_InsertItem(GetDlgItem(hdlg,IDC_LIST1),&item1);
						item1.iSubItem = 1;
						wsprintf(Listfiles[item1.iItem].szFileName,"%s",szFileName);
						item1.pszText = (LPTSTR)Listfiles[item1.iItem].szFileName;
						ListView_SetItem(GetDlgItem(hdlg,IDC_LIST1),&item1);
						if(Listfiles[item1.iItem].img->LoadBMP(szFileName) == FALSE)
						MessageBox(hdlg,"화일이 없슴다. 내 맘대로 하겠슴다.","메롱",MB_YESNO|MB_ICONQUESTION);
					}
				break;
			}
			return TRUE;

			case WM_NOTIFY:
			{
				NMHDR *nmdr;
				LPNM_LISTVIEW infox;
				nmdr = (LPNMHDR)lParam;
				if(wParam == IDC_LIST2)
				{
					infox = (LPNM_LISTVIEW) lParam;
					switch(nmdr->code)
					{
						case NM_SETFOCUS:
								InList2=TRUE;
							break;

						case NM_CLICK :
							  if(NEditpos != NULL)
							  {
								L2Click = infox->iItem;
							    if(L2Click < NEditpos->MaxMotion && L2Click > -1)
								{
//									char yy[50];
//									wsprintf(yy,"%d",L2Click);
//									MessageBox(NULL,yy,MB_OK, NULL);
								    m_BK->PutImageInWorld(NEditpos->MotionList[L2Click].charpoint.x,NEditpos->MotionList[L2Click].charpoint.y,
									     Listfiles[NEditpos->MotionList[L2Click].ListFileIndex].img->width,
									     Listfiles[NEditpos->MotionList[L2Click].ListFileIndex].img->height,
								      	 Listfiles[NEditpos->MotionList[L2Click].ListFileIndex].img->image);
									SendDlgItemMessage(hdlg, IDC_COMBO1,CB_SETCURSEL,NEditpos->MotionList[L2Click].delay-1,0);
								}
							  }
							break;
					}
				}
				if(wParam == IDC_LIST1)
				{
					infox = (LPNM_LISTVIEW) lParam;
					switch(nmdr->code)
					{
						case NM_DBLCLK: // 더블 클릭
							m_BK2->CLOSE = TRUE;
							m_BK2->realvirsize.x = Listfiles[infox->iItem].img->width;
							m_BK2->realvirsize.y = Listfiles[infox->iItem].img->height;
							m_BK2->m_realvir = Listfiles[infox->iItem].img->image;
							InvalidateRect(m_BK2->Dibhwnd,NULL,FALSE);
							//m_BK2->PutImage(0,0,Listfiles[infox->iItem].img->width,Listfiles[infox->iItem].img->height,Listfiles[infox->iItem].img->image);
						break;

						///////////////////////////////
						// 드래그
						case LVN_BEGINDRAG:
							{
								ImageList_BeginDrag(hDragList,0,infox->ptAction.x-pt.x,infox->ptAction.y-pt.y);
								ImageList_DragEnter(GetDlgItem(hdlg,IDC_LIST1),infox->ptAction.x,infox->ptAction.y);
								SetCapture(hdlg);
								pt = infox->ptAction;
								nDragItem = infox->iItem;
								IsDrag = TRUE;
							}
						break;

						///////////////////////////////
						// 키입력
						case LVN_KEYDOWN:
							{
								LV_KEYDOWN *keys;
								keys = (LV_KEYDOWN *) lParam;
								switch(keys->wVKey)
								{
									case VK_DELETE:
									break;
									case VK_RETURN:
									break;
								}
							}
						break;

					}
				}
			}
			break;

		case WM_MOUSEMOVE:
			{
				if(IsDrag) {
					SetCursor(LoadCursor(hInst,"DragCursor"));
					ImageList_DragMove(LOWORD(lParam),HIWORD(lParam));
				}
			}
			break;

		case WM_LBUTTONUP:
				if(IsDrag)
				{
					POINT ptPos;

					ImageList_DragLeave(GetDlgItem(hdlg,IDC_LIST1));
					ImageList_EndDrag();
					ReleaseCapture();
  					GetCursorPos(&ptPos);            /* 마우스위치를 잡는다. */
					ScreenToClient(hdlg,&ptPos);     /* 현 윈도우위치의 마우스 좌표로 재인식 시킨다.  */
					if(ptPos.x>548 && ptPos.y>239 && ptPos.x<830 && ptPos.y<465)
					{
						if(NEditpos != NULL)
						{
							LV_ITEM item1;
							item1.mask = LVIF_TEXT;
							int nCur= -1;
							while((nCur=ListView_GetNextItem(GetDlgItem(hdlg,IDC_LIST1),nCur,LVNI_SELECTED)) > -1)
							{
								AddMotionList(nCur,1);
							}
							InList2=FALSE;
						} else MessageBox(NULL,"Dance title을 모르겠슴따. 만드십쇼!", MB_OK, NULL);
					} 
					IsDrag = FALSE;
				}
			break;

		case WM_CLOSE:
			DestroyWindow(hdlg);
			hdlg=0;
			break;

	}
	return FALSE;
}


////////////////////////////////////////////////////////////////////
//
//

LRESULT	CALLBACK InputProc(HWND hdlg,UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case WM_INITDIALOG:
					SetFocus(GetDlgItem(hdlg,IDC_EDIT1));
				return TRUE;

				case IDOK:
					{
						char  SZbuffname[256];
						int   iLength = SendDlgItemMessage(hdlg, IDC_EDIT1,EM_GETLINE,0,(LPARAM)SZbuffname);
						SZbuffname[iLength] = 0;
						//wsprintf(SZbuffname,"%d",iLength);
						AddDanceData(SZbuffname);
						SendMessage(modalhwnd,WM_COMMAND,USER_GETINPUT,0);
						EndDialog(hdlg,IDOK);
					}
					break;
				case IDCANCEL:
						EndDialog(hdlg,IDCANCEL);
					break;
			}
			return TRUE;
	}

	return FALSE;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  트랙바용 함수들.
//

void InitTrackBar(int MaxiumNum)
{
	SendDlgItemMessage(modalhwnd, IDC_SLIDER1,TBM_SETRANGE,TRUE,MAKELPARAM(0,MaxiumNum));
	SendDlgItemMessage(modalhwnd, IDC_SLIDER1,TBM_CLEARSEL,TRUE,0);
	SendDlgItemMessage(modalhwnd, IDC_SLIDER1,TBM_CLEARTICS,TRUE,0);
}

void MoveTrackBar(int nPos)
{
	SendDlgItemMessage(modalhwnd, IDC_SLIDER1,TBM_SETPOS,TRUE,nPos);
}

