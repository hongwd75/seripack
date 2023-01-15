// visualMask.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "background.h"
#include <stdlib.h>
#include <stdio.h>
#include "resource.h"
#include "file.h"


BG		*PIC1;
HMENU				 hMenu;      // ¸Þ´º¿ë
HINSTANCE			 hInst;
HWND				 hwnd;


long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void InitMySystem(void)
{
	PIC1 = new BG(10,5,640,480,FALSE,hwnd,hInst);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	static char szAppName[] = "Visual Image Masker";

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

	InitMySystem();

    ShowWindow( hwnd, nCmdShow);
    UpdateWindow(hwnd); 

    while(GetMessage( &msg, NULL, 0, 0) ) 
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
   }

   return msg.wParam;

}




void m_ReadImageFile()
{
	static char szFileName[_MAX_PATH];
	static char szTitleName[_MAX_FNAME + _MAX_EXT];

	szFileName[0]='\0';
	szTitleName[0]='\0';
	if(PopFileOpenDlg(hwnd,szFileName,szTitleName))
	{
		if(PIC1->ReadBMPfile(szFileName))
		{
			InvalidateRect(PIC1->Dibhwnd,NULL,FALSE);
		}
	}

}
long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	static char szFileName[_MAX_PATH];
	static char szTitleName[_MAX_FNAME + _MAX_EXT];
	char hh[50];

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
				
				case IDM_BMPOPEN:
					m_ReadImageFile();
				return 0;

				case IDM_SAVEMASK:
					if(PopLISTFileSaveDlg(hwnd,szFileName,szTitleName))
					{
						FILE *fp;
						fp = fopen(szFileName,"wb");
						int px = PIC1->maskwidth,py = PIC1->maskheight;
						fwrite(&px,4,1,fp);
						fwrite(&py,4,1,fp);
						fwrite(PIC1->MASK16,px*py,1,fp);
						fclose(fp);
					}
				return 0;

				case IDM_AUTOMASK:
					PIC1->AutoMask();
					InvalidateRect(PIC1->Dibhwnd,NULL,FALSE);	
				return 0;
			}
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
					PostMessage(PIC1->Dibhwnd, WM_CLOSE, 0, 0);
					delete PIC1;
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
