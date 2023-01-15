// MapEditer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "stdafx.h"

#include "stdafx.h"
#include "background.h"
#include <stdlib.h>
#include <stdio.h>
#include "resource.h"
#include "file.h"




LAYERTYPE maindata;
BG       *Layers;
BG       *Scren;

HMENU				 hMenu;      // ¸Þ´º¿ë
HINSTANCE			 hInst;
HWND				 hwnd,Tilehwnd;
int					 img_width,img_height;
char                 Listfilepath[256];

extern inmain		InObjectlist;
extern outmain		OutObjectlist;


mhsystem wondae;


void SaveWorkSpace(void);
void ViewTiles(void);
void SetTileLayer(int layer);  // first 0 ..
void InsertImageFile(LPSTR filename);
void GetPath(LPSTR source);
RGBstruct* ReadBMPfile(char *filename,int kk);

extern BOOL    SELECT;
BOOL	opened = FALSE;


//extern RGBstruct* ReadBMPfile(char *filename);
BOOL CALLBACK DigProc(HWND,UINT,WPARAM,LPARAM);
long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void InitMySystem(void)
{
   Scren = new BG(5,5,640,480,TRUE,hwnd,hInst);  
   Scren->NoScroll = TRUE;
   Scren->GetVirMem(650,500);
   Scren->Is_Haveimage = FALSE;
   wondae.viewlayer[0] = FALSE;
   wondae.viewlayer[1] = FALSE;
   wondae.viewlayer[2] = FALSE;
   wondae.viewlayer[3] = FALSE;
   wondae.viewlayer[4] = FALSE;
   wondae.worklayer = -1;
   wondae.showx=0;wondae.showy=0;
   
}


void SaveWorkSpace();

void DestroyMySystem(void)
{
	delete Scren;
	delete Layers;
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	static char szAppName[] = "Visual Map Editor";

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
		hwnd = CreateWindow(szAppName,szAppName,WS_BORDER | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,0,0,680,550,NULL,NULL,hInst,NULL);
	}

	Tilehwnd = CreateDialog(hInst,"IDD_DIALOG1",hwnd,DigProc);
	
	InitMySystem();
	Layers = new BG(10,10,640,416,FALSE,Tilehwnd,hInst);  

    ShowWindow( hwnd, nCmdShow);
	ShowWindow(Tilehwnd,nCmdShow);
    UpdateWindow(hwnd); 

    while(GetMessage( &msg, NULL, 0, 0) ) 
	{
		if(Tilehwnd == 0 || !IsDialogMessage(Tilehwnd,&msg))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
   }

   return msg.wParam;

}




long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	static char szFileName[_MAX_PATH];
	static char szTitleName[_MAX_FNAME + _MAX_EXT];

    switch (message)
    {
		case WM_CREATE:
				PopFileInit(hWnd);
				InitCommonControls();
				hMenu = GetMenu(hWnd);
			return 0;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				
				case IDM_OPEN:
					szFileName[0]='\0';
					szTitleName[0]='\0';

					if(PopFileOpenDlg(hWnd,szFileName,szTitleName))
					{
						InsertImageFile(szFileName);
					}
					return 0;

				case IDM_SAVESPACE:
						SaveWorkSpace();
					return 0;
			}
		break;

        case WM_KEYDOWN:
            switch (wParam)
            {
                case VK_LEFT:
						if(wondae.showx>0)wondae.showx--;
						ViewTiles();
                    break;
                case VK_RIGHT:
						if(wondae.showx<maindata.mapsize[wondae.worklayer].x-1)wondae.showx++;
						ViewTiles();
                    break;
                case VK_UP:
						if(wondae.showy>0)wondae.showy--;
						ViewTiles();
                    break;
                case VK_DOWN:
						if(wondae.showy<maindata.mapsize[wondae.worklayer].y-1)wondae.showy++;
						ViewTiles();
                    break;
            }
			InvalidateRect(Scren->Dibhwnd,NULL,FALSE);
            return 0;

		case WM_PAINT:
			{
			}
		break; 

		case WM_CLOSE:
			{
				int askx = MessageBox(hWnd,"Do you want to close PROGRAM ?","CLOSE",MB_YESNO|MB_ICONQUESTION);
				if(askx == IDYES) 
				{
				//	PostMessage(PIC1->Dibhwnd, WM_CLOSE, 0, 0);
				//	delete PIC1;
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

BOOL CALLBACK DigProc(HWND hdlg,UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_INITDIALOG:
		{
			SendDlgItemMessage(hdlg, IDC_VIEWL1,BM_SETCHECK,0,0);
			SendDlgItemMessage(hdlg, IDC_VIEWL2,BM_SETCHECK,0,0);
			SendDlgItemMessage(hdlg, IDC_VIEWL3,BM_SETCHECK,0,0);
			SendDlgItemMessage(hdlg, IDC_VIEWL4,BM_SETCHECK,0,0);
			SendDlgItemMessage(hdlg, IDC_VIEWL5,BM_SETCHECK,0,0);
			SendDlgItemMessage(hdlg, IDC_WORK1,BM_SETCHECK,0,0);
			SendDlgItemMessage(hdlg, IDC_WORK2,BM_SETCHECK,0,0);
			SendDlgItemMessage(hdlg, IDC_WORK3,BM_SETCHECK,0,0);
			SendDlgItemMessage(hdlg, IDC_WORK4,BM_SETCHECK,0,0);
			SendDlgItemMessage(hdlg, IDC_WORK5,BM_SETCHECK,0,0);
		}
		return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_VIEWL1:
					SendDlgItemMessage(hdlg, IDC_VIEWL1,BM_SETCHECK,0,0);
					if(wondae.viewlayer[0])
					{
						wondae.viewlayer[0] = FALSE;
					} else 
					{
						if(maindata.havetile[0])
						{
							wondae.viewlayer[0] = TRUE;
							SendDlgItemMessage(hdlg, IDC_VIEWL1,BM_SETCHECK,1,0);
						}
					}
					ViewTiles();
					break;
				case IDC_VIEWL2:
					SendDlgItemMessage(hdlg, IDC_VIEWL2,BM_SETCHECK,0,0);
					if(wondae.viewlayer[1])
					{
						wondae.viewlayer[1] = FALSE;
					} else 
					{
						if(maindata.havetile[1])
						{
							wondae.viewlayer[1] = TRUE;
							SendDlgItemMessage(hdlg, IDC_VIEWL2,BM_SETCHECK,1,0);
						}
					}
					ViewTiles();
					break;
				case IDC_VIEWL3:
					SendDlgItemMessage(hdlg, IDC_VIEWL3,BM_SETCHECK,0,0);
					if(wondae.viewlayer[2])
					{
						wondae.viewlayer[2] = FALSE;
					} else 
					{
						if(maindata.havetile[2])
						{
							wondae.viewlayer[2] = TRUE;
							SendDlgItemMessage(hdlg, IDC_VIEWL3,BM_SETCHECK,1,0);
						}
					}
					ViewTiles();
					break;
				case IDC_VIEWL4:
					SendDlgItemMessage(hdlg, IDC_VIEWL4,BM_SETCHECK,0,0);
					if(wondae.viewlayer[3])
					{
						wondae.viewlayer[3] = FALSE;
					} else 
					{
						if(maindata.havetile[3])
						{
							wondae.viewlayer[3] = TRUE;
							SendDlgItemMessage(hdlg, IDC_VIEWL4,BM_SETCHECK,1,0);
						}
					}
					ViewTiles();
					break;
				case IDC_VIEWL5:
					SendDlgItemMessage(hdlg, IDC_VIEWL5,BM_SETCHECK,0,0);
					if(wondae.viewlayer[4])
					{
						wondae.viewlayer[4] = FALSE;
					} else 
					{
						wondae.viewlayer[4] = TRUE;
						SendDlgItemMessage(hdlg, IDC_VIEWL5,BM_SETCHECK,1,0);
					}
					ViewTiles();
					break;

				case IDC_WORK1:
					{
						SendDlgItemMessage(hdlg, IDC_WORK1,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK2,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK3,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK4,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK5,BM_SETCHECK,0,0);
						if(maindata.havetile[0])
						{
							SendDlgItemMessage(hdlg, LOWORD(wParam),BM_SETCHECK,1,0);
							wondae.worklayer = 0;
							SetTileLayer(wondae.worklayer);
							SELECT = FALSE;
						} else
						{
							switch(wondae.worklayer)
							{
								case 0:
									SendDlgItemMessage(hdlg, IDC_WORK1,BM_SETCHECK,1,0);
									break;
								case 1:
									SendDlgItemMessage(hdlg, IDC_WORK2,BM_SETCHECK,1,0);
									break;
								case 2:
									SendDlgItemMessage(hdlg, IDC_WORK3,BM_SETCHECK,1,0);
									break;
								case 3:
									SendDlgItemMessage(hdlg, IDC_WORK4,BM_SETCHECK,1,0);
									break;
								case 4:
									SendDlgItemMessage(hdlg, IDC_WORK5,BM_SETCHECK,1,0);
									break;
							}
						}
					}
					break;
				case IDC_WORK2: 
					{
						SendDlgItemMessage(hdlg, IDC_WORK1,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK2,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK3,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK4,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK5,BM_SETCHECK,0,0);
						if(maindata.havetile[1])
						{
							SendDlgItemMessage(hdlg, LOWORD(wParam),BM_SETCHECK,1,0);
							wondae.worklayer = 1;
							SetTileLayer(wondae.worklayer);
							SELECT = FALSE;
						} else
						{
							switch(wondae.worklayer)
							{
								case 0:
									SendDlgItemMessage(hdlg, IDC_WORK1,BM_SETCHECK,1,0);
									break;
								case 1:
									SendDlgItemMessage(hdlg, IDC_WORK2,BM_SETCHECK,1,0);
									break;
								case 2:
									SendDlgItemMessage(hdlg, IDC_WORK3,BM_SETCHECK,1,0);
									break;
								case 3:
									SendDlgItemMessage(hdlg, IDC_WORK4,BM_SETCHECK,1,0);
									break;
								case 4:
									SendDlgItemMessage(hdlg, IDC_WORK5,BM_SETCHECK,1,0);
									break;
							}
						}
					}
					break;

				case IDC_WORK3: 
					{
						SendDlgItemMessage(hdlg, IDC_WORK1,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK2,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK3,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK4,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK5,BM_SETCHECK,0,0);
						if(maindata.havetile[2])
						{
							SendDlgItemMessage(hdlg, LOWORD(wParam),BM_SETCHECK,1,0);
							wondae.worklayer = 2;
							SetTileLayer(wondae.worklayer);
							SELECT = FALSE;
						} else
						{
							switch(wondae.worklayer)
							{
								case 0:
									SendDlgItemMessage(hdlg, IDC_WORK1,BM_SETCHECK,1,0);
									break;
								case 1:
									SendDlgItemMessage(hdlg, IDC_WORK2,BM_SETCHECK,1,0);
									break;
								case 2:
									SendDlgItemMessage(hdlg, IDC_WORK3,BM_SETCHECK,1,0);
									break;
								case 3:
									SendDlgItemMessage(hdlg, IDC_WORK4,BM_SETCHECK,1,0);
									break;
								case 4:
									SendDlgItemMessage(hdlg, IDC_WORK5,BM_SETCHECK,1,0);
									break;
							}
						}
					}
					break;

				case IDC_OBJECTCOMBO1:
					if(HIWORD(wParam) == CBN_SELCHANGE) 
					{
						int ImageTypeSel = SendDlgItemMessage(hdlg, IDC_OBJECTCOMBO1,CB_GETCURSEL,0,0);
						SendDlgItemMessage(hdlg, IDC_OBJECTCOMBO1,CB_SETCURSEL,ImageTypeSel,0);
						InObjectlist.selobject = ImageTypeSel;
						if(wondae.worklayer == 4) SetTileLayer(4);
					}
					return 0;

				case IDC_WORK4: 
					{
						SendDlgItemMessage(hdlg, IDC_WORK1,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK2,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK3,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK4,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK5,BM_SETCHECK,0,0);
						if(maindata.havetile[3])
						{
							SendDlgItemMessage(hdlg, LOWORD(wParam),BM_SETCHECK,1,0);
							wondae.worklayer = 3;
							SetTileLayer(wondae.worklayer);
							SELECT = FALSE;
						} else
						{
							switch(wondae.worklayer)
							{
								case 0:
									SendDlgItemMessage(hdlg, IDC_WORK1,BM_SETCHECK,1,0);
									break;
								case 1:
									SendDlgItemMessage(hdlg, IDC_WORK2,BM_SETCHECK,1,0);
									break;
								case 2:
									SendDlgItemMessage(hdlg, IDC_WORK3,BM_SETCHECK,1,0);
									break;
								case 3:
									SendDlgItemMessage(hdlg, IDC_WORK4,BM_SETCHECK,1,0);
									break;
								case 4:
									SendDlgItemMessage(hdlg, IDC_WORK5,BM_SETCHECK,1,0);
									break;
							}
						}
					}
					break;
				case IDC_WORK5: 
					{
						SendDlgItemMessage(hdlg, IDC_WORK1,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK2,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK3,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK4,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, IDC_WORK5,BM_SETCHECK,0,0);
						SendDlgItemMessage(hdlg, LOWORD(wParam),BM_SETCHECK,1,0);
						wondae.worklayer = 4;
						SetTileLayer(wondae.worklayer);
						SELECT = FALSE;
					}
					break;
			}

		return TRUE;

		case WM_CLOSE:
			DestroyWindow(hdlg);
			hdlg=0;
			break;
	}
	return FALSE;
}
