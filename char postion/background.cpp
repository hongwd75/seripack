#include"stdafx.h"
#include"background.h"
#include<stdio.h>
#include<io.h>


extern HCURSOR		  m_cross;

LRESULT CALLBACK DIB_WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
int BG::m_MakeScrolBar = 0;
BG::BG(int Posx,int Posy,int ScSizeX,int ScSizeY,BOOL HaveScroll,HWND hWndParent,HINSTANCE hInst)
{

	// 차일드 윈도우 구성

	m_Enlarge		= 1;
	m_backimg		= NULL;
	m_virImg		= NULL;
	CLOSE			= TRUE;
	Is_Haveimage	= FALSE;
	wsprintf(szName,"DIB_Window");
	WNDCLASSEX wndclass;
	hwndParent = hWndParent;
	wndclass.cbSize        = sizeof(wndclass);
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = DIB_WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = sizeof(LONG);
	wndclass.hInstance     = hInst;
	wndclass.hIcon         = NULL;
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szName;
	wndclass.hIconSm       = NULL;
	RegisterClassEx(&wndclass);
	
	if(HaveScroll)
	Dibhwnd = CreateWindow(szName,                    // window class name
						   szName,   // window caption
						WS_CHILD | WS_BORDER | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL , 
						//WS_CHILD | WS_BORDER | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL| ES_AUTOHSCROLL | ES_AUTOVSCROLL ,
						0,
						0,
						0,
						0,
						hWndParent,           // parent window handle
						NULL,                 // window menu handle
						hInst,                // program instance handle
						NULL);                // creation parameters
	else 
	Dibhwnd = CreateWindow(szName,                    // window class name
						   szName,   // window caption
						//WS_CHILD | WS_BORDER | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_THICKFRAME,
						WS_CHILD | WS_BORDER | WS_VISIBLE,
						0,
						0,
						0,
						0,
						hWndParent,           // parent window handle
						NULL,                 // window menu handle
						hInst,                // program instance handle
						NULL);                // creation parameters
	
	if(Dibhwnd==NULL)
	{
		MessageBox(hWndParent,"DIB창을 만들수없습니다.","Error",MB_OK);
		return;
	}
	SetWindowLong(Dibhwnd, GWL_USERDATA, (long)this );
	m_HaveScrol = HaveScroll;
	m_ScreenSize.x = ScSizeX;
	m_ScreenSize.y = ScSizeY;
	m_ScreenPos.x = Posx;
	m_ScreenPos.y = Posy;
	m_ScreenNPos.x = 0;
	m_ScreenNPos.y = 0;
	width = ScSizeX;
	height = ScSizeY;
	m_back_handle = GlobalAlloc(GHND,sizeof(BITMAPINFO));
	m_backround = (LPBITMAPINFO)GlobalLock(m_back_handle);
	m_backround->bmiHeader.biSize = 40;
	m_backround->bmiHeader.biWidth = m_ScreenSize.x;
	m_backround->bmiHeader.biHeight = -m_ScreenSize.y;
	m_backround->bmiHeader.biPlanes = 1;
	m_backround->bmiHeader.biBitCount = 24;
	m_backround->bmiHeader.biCompression = 0;
	m_backround->bmiHeader.biSizeImage = 0;
	m_backround->bmiHeader.biXPelsPerMeter = 0;
	m_backround->bmiHeader.biYPelsPerMeter = 0;
	m_backround->bmiHeader.biClrUsed = 0;
	m_backround->bmiHeader.biClrImportant = 0;
	m_backimg = new RGBstruct[ScSizeX*ScSizeY];
	for(int i=0;i<ScSizeX*ScSizeY;i++)	
	{
		m_backimg[i].R = 0;
		m_backimg[i].G = 0;
		m_backimg[i].B = 100;
	}
	if(m_HaveScrol) MoveWindow(Dibhwnd,Posx,Posy,ScSizeX+18,ScSizeY+18,FALSE);
	else MoveWindow(Dibhwnd,Posx,Posy,ScSizeX+1,ScSizeY+1,FALSE);
	m_ViewCross = FALSE;
	m_ImageCurs = FALSE;
	Worldpoint.x = ScSizeX >> 1;		// 화면 중간에 world point 구성
	Worldpoint.y = ScSizeY - 30;		// 상동.
	m_resizewindow = FALSE;
}



BG::~BG()
{
	CLOSE = FALSE;
	GlobalUnlock(m_back_handle);
	GlobalFree(m_back_handle);
	if(m_backimg!=NULL) delete(m_backimg);
	if(Is_Haveimage) delete(m_virImg);
}




void BG::PutBackGround(HDC hdc)
{
	if(CLOSE)
	{
//		if(!m_resizewindow)
		StretchDIBits(hdc,0,0,m_backround->bmiHeader.biWidth,-(m_backround->bmiHeader.biHeight),//
		0,0,m_backround->bmiHeader.biWidth,-(m_backround->bmiHeader.biHeight), m_backimg, m_backround,DIB_RGB_COLORS,SRCCOPY);
//		else
//			StretchDIBits(hdc,0,0,m_backround->bmiHeader.biWidth,-m_backround->bmiHeader.biHeight,//
//			0,0,realvirsize.x,realvirsize.y, m_realvir, m_backround,DIB_RGB_COLORS,SRCCOPY);

	}
}



void BG::m_PutImage2BackBuffer()
{
	int Lwidth,ScCount1=0,ScCount2=0,ImgCount1,ImgCount2;

	if(!Is_Haveimage) return;

	RGBstruct *temp = m_virImg;										// 포인터 전달. 

	ImgCount1=m_ScreenNPos.y*width+m_ScreenNPos.x;					// 위치 계산. 

	for(int Lheight=m_ScreenNPos.y;Lheight<height;Lheight++)
	{
		ImgCount2 = ImgCount1;
		ScCount2 = ScCount1;
		for(Lwidth=m_ScreenNPos.x;Lwidth<width;Lwidth++)
		{
			m_backimg[ScCount2].R = temp[ImgCount2].R;
			m_backimg[ScCount2].G = temp[ImgCount2].G;
			m_backimg[ScCount2++].B = temp[ImgCount2++].B;
			if(Lwidth - m_ScreenNPos.x > m_ScreenSize.x ) break;
		}
		ImgCount1+=width;
		ScCount1+=m_ScreenSize.x;
		if(Lheight - m_ScreenNPos.y > m_ScreenSize.y ) break;
	}
}



void BG::WindowLen(void)
{
	if(m_HaveScrol)
	{
		SetScrollRange(Dibhwnd,SB_VERT,0,height*m_Enlarge - m_ScreenSize.y,TRUE);
		SetScrollRange(Dibhwnd,SB_HORZ,0,width*m_Enlarge - m_ScreenSize.x,TRUE);
		SetScrollPos(Dibhwnd,SB_VERT,m_ScreenNPos.y*m_Enlarge,FALSE);
		SetScrollPos(Dibhwnd,SB_HORZ,m_ScreenNPos.x*m_Enlarge,FALSE);
	}
}


void BG::WindowEnlarge(void)			// 그림 확대
{
	m_Enlarge++;
	WindowLen();
}

void BG::WindowReduce(void)				// 그림 축소
{
	if(m_Enlarge > 1) m_Enlarge--;
	WindowLen();
}



/*
 ====================================================================

  FILL BACK BUFFER

 ====================================================================
*/
void BG::FillBackBuffer(int r,int g,int b)
{
	int i;
	for(i=0;i<m_ScreenSize.x*m_ScreenSize.y;i++)
	{
		m_backimg[i].R = r;
		m_backimg[i].G = g;
		m_backimg[i].B = b;
	}

}



/*
 ====================================================================

  WINDOW RESIZE

 ====================================================================
*/
void BG::WindowReSize(void)
{
	int cx,cy;
	RECT rWindow;

	GetWindowRect(Dibhwnd,&rWindow);
	cy = rWindow.bottom - rWindow.top;
	cx = rWindow.right - rWindow.left;
	if(m_HaveScrol)
	{
		cx-=18;		// 스크롤바 자리 
		cy-=18;     // 스크롤바 자리
		WindowLen();
	}
    m_ScreenSize.x = cx;
	m_ScreenSize.y = cy;
	m_backround->bmiHeader.biWidth = m_ScreenSize.x;
	m_backround->bmiHeader.biHeight = -m_ScreenSize.y;

	if(m_backimg != NULL) 
	{
		delete m_backimg;
	}

	m_backimg = new RGBstruct[cx*cy];
}



void BG::MakeVirMemory(int x,int y)
{

	int i;
	if(Is_Haveimage) 
	{
		Is_Haveimage = FALSE;
		delete m_virImg;
	}
	m_virImg = new RGBstruct[x * y];
	width = x;
	height = y;
	CLOSE = TRUE;
	Is_Haveimage = TRUE;
	m_ScreenNPos.x = 0;
	m_ScreenNPos.y = 0;
	WindowLen();
	for(i=0;i<x*y;i++)
	{
		m_virImg[i].R = 0;
		m_virImg[i].G = 0;
		m_virImg[i].B = 0;
	}
	FillBackBuffer(0,0,100);
	m_PutImage2BackBuffer();
	InvalidateRect(hwndParent,NULL,FALSE);
}



/////////////////////////////////////////////////////////////////////////////
//  월드 좌표계에 대입
// 
void BG::PutImageInWorld(int x,int y, int width1,int height1, RGBstruct *src)  // 월드 좌표에 대입
{
	int l1,srcad,tag=0;
	int xsize=width1,ysize=height1;
	RGBstruct *tt=m_virImg,*ss=src;

	x = x + Worldpoint.x - (width1 >> 1);
	y = y + Worldpoint.y - (height1);
	if(x<0) 
	{
		xsize = width1 + x;
		tag -= x;
		x = 0;

	}
	if(x + xsize > width - 1) xsize -= (x+xsize - width);

	if(y<0) 
	{
		ysize = height1 + y;
		tag -= width1 * y;
		y = 0;

	}
	if(y + ysize > height - 1) ysize -= (y+ysize - height);
	srcad = width * y + x;
	for(l1=0;l1<ysize;l1++)
	{
		memcpy(tt+srcad,ss+tag,xsize*3);
		srcad += width;
		tag += width1;
	}
	InvalidateRect(Dibhwnd,NULL,FALSE);
}



/////////////////////////////////////////////////////////////////////////////
//  일반 좌표계에 대입
// 
void BG::PutImage(int x,int y, int width1,int height1, RGBstruct *src)
{
	int l1,srcad,tag=0;
	int xsize=width1,ysize=height1;

	RGBstruct *tt=m_virImg,*ss=src;
	
	
	if(x<0) 
	{
		xsize = width1 + x;
		tag -= x;
		x = 0;
	}
	if(x + xsize > width - 1) xsize -= (x+xsize - width);
	if(y<0) 
	{
		ysize = height1 + y;
		tag -= width1 * y;
		y = 0;

	}
	if(y + ysize > height - 1) ysize -= (y+ysize - height);
	srcad = width * y + x;
	for(l1=0;l1<ysize;l1++)
	{
		memcpy(tt+srcad,ss+tag,xsize*3);
		srcad += width;
		tag += width1;
	}
	InvalidateRect(Dibhwnd,NULL,FALSE);
}



void BG::PutDataVirMemory(BYTE rgb,int addr)
{
	m_virImg[addr].R = rgb;
	m_virImg[addr].G = rgb;
	m_virImg[addr].B = rgb;
	InvalidateRect(Dibhwnd,NULL,FALSE);
}



LRESULT CALLBACK DIB_WndProc(HWND hwnD, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	BG *Win = (BG *)GetWindowLong(hwnD, GWL_USERDATA);
	static HPEN hPen;
	static HPEN hPen2;

	switch(iMsg)
	{

		case WM_COMMAND :
			if(Win == NULL) return 0;
			hPen = CreatePen(PS_SOLID,1,RGB(100,255,255));
			hPen2 = CreatePen(PS_DOT,1,RGB(255,255,100));
			return 0;

		case WM_SIZE :
			{
				if(Win == NULL) return 0;
				InvalidateRect(hwnD,NULL,FALSE);
			}
			return 0;

		case WM_CLOSE:
			{
					Win->CLOSE = FALSE;
					if(Win->Is_Haveimage) 
					{
						Win->Is_Haveimage = FALSE;
						delete Win->m_virImg;
					}
					DestroyWindow(hwnD);
			}
			return 0;

        case WM_DESTROY:
             PostQuitMessage(0);
            return 0;
			
		case WM_PAINT :
			{
				RECT rect;
				PAINTSTRUCT ps;
				Win->m_PutImage2BackBuffer();
				BeginPaint(Win->Dibhwnd,&ps);
				GetClientRect(hwnD,&rect);
				Win->PutBackGround(ps.hdc);
				//m_ImageSize.x
				if(Win->m_ViewCross)
				{
					SelectObject(ps.hdc,hPen);
					MoveToEx(ps.hdc,Win->m_ScreenSize.x >> 1,Win->m_ScreenSize.y-50,NULL);
					LineTo(ps.hdc,Win->m_ScreenSize.x >> 1,Win->m_ScreenSize.y-10);
					MoveToEx(ps.hdc,(Win->m_ScreenSize.x >> 1)-20,Win->m_ScreenSize.y-30,NULL);
					LineTo(ps.hdc,(Win->m_ScreenSize.x >> 1)+20,Win->m_ScreenSize.y-30);
				}
				if(Win->m_ImageCurs)
				{
					int x,y;
					SelectObject(ps.hdc,hPen2);
					x =  Win->Worldpoint.x + ( Win->Addpoint.x);
					y =  Win->Worldpoint.y + (Win->Addpoint.y);
					MoveToEx(ps.hdc,x - (Win->m_ImageSize.x >> 1), y - Win->m_ImageSize.y,NULL);
					LineTo(ps.hdc,x - (Win->m_ImageSize.x >> 1), y);
					LineTo(ps.hdc,x + (Win->m_ImageSize.x >> 1), y);
					LineTo(ps.hdc,x + (Win->m_ImageSize.x >> 1), y - Win->m_ImageSize.y);
					LineTo(ps.hdc,x - (Win->m_ImageSize.x >> 1), y - Win->m_ImageSize.y);
				}
				EndPaint(Win->Dibhwnd,&ps);
			}
			return 0;


		case WM_MOUSEMOVE:
			if(Win->m_ImageCurs)
			{
				SetCursor(m_cross);
				Win->Addpoint.x = LOWORD(lParam) - Win->Worldpoint.x;
				Win->Addpoint.y = HIWORD(lParam) - Win->Worldpoint.y;
				InvalidateRect(hwnD,NULL,FALSE);
			}
			return 0;

		case WM_LBUTTONUP:
			if(Win->m_ImageCurs)
			{
				SendMessage(Win->hwndParent,WM_COMMAND,USER_GETPOS,(LPARAM)(((Win->Addpoint.y) << 16) + Win->Addpoint.x));
				Win->m_ViewCross = FALSE;
				Win->m_ImageCurs = FALSE;
			}
			return TRUE;

		case WM_VSCROLL:
			switch(LOWORD(wParam))
			{
				case SB_PAGEDOWN :
					Win->m_ScreenNPos.y=min(Win->height - Win->m_ScreenSize.y,Win->m_ScreenNPos.y+10);
					break;
				case SB_LINEDOWN:
					Win->m_ScreenNPos.y=min(Win->height - Win->m_ScreenSize.y,Win->m_ScreenNPos.y+1);
					break;
				case SB_PAGEUP :
					Win->m_ScreenNPos.y=max(0,Win->m_ScreenNPos.y-10);
					break;
				case SB_LINEUP:
					Win->m_ScreenNPos.y=max(0,Win->m_ScreenNPos.y-1);
					break;
			}
			SetScrollPos(hwnD,SB_VERT,Win->m_ScreenNPos.y,TRUE);
			InvalidateRect(hwnD,NULL,FALSE);
		return 0;

		case WM_HSCROLL:
			switch(LOWORD(wParam))
			{
				case SB_PAGEDOWN :
					Win->m_ScreenNPos.x=min(Win->width - Win->m_ScreenSize.x,Win->m_ScreenNPos.x+10);
					break;
				case SB_LINEDOWN:
					Win->m_ScreenNPos.x=min(Win->width - Win->m_ScreenSize.x,Win->m_ScreenNPos.x+2);
					break;
				case SB_PAGEUP :
					Win->m_ScreenNPos.x=max(0,Win->m_ScreenNPos.x-10);
					break;
				case SB_LINEUP:
					Win->m_ScreenNPos.x=max(0,Win->m_ScreenNPos.x-2);
					break;
			}
			SetScrollPos(hwnD,SB_HORZ,Win->m_ScreenNPos.x,TRUE);
			InvalidateRect(hwnD,NULL,FALSE);
		return 0;

	}
	return DefWindowProc(hwnD, iMsg, wParam, lParam);
}



/////////////////////////////////////////////////////////////////////////////////////////////
//
//

WORD GetColorValue(int R,int G, int B)
{
	return ((R >> 3) <<10) | ((G >> 3) << 5) | (B >> 3);
}

ClImage::ClImage()
{
	image = NULL;
}

////////////////////////////////////////////////////////////////////////
// 죽여~~ 죽여버려
//
ClImage::~ClImage()
{
	if(image != NULL) delete image;
}



///////////////////////////////////////////////////////////////////////
//   이지를 로뜨하면서 생성한다네
//
ClImage::ClImage(LPSTR filename)
{
	LoadBMP(filename);
}


///////////////////////////////////////////////////////////////////////
//  16삐뜨 이미지루 변해서 주지.
//
WORD* ClImage::Get16BitImage()
{
	int loop;
	RGBstruct *images = this->image;
	WORD *Virimage16 = new WORD[this->height * this->width];

	for(loop = 0; loop < this->width * this->height; loop++)
	{
		Virimage16[loop] = GetColorValue(images->R,images->G,images->B);
		images++;
	}              
	return Virimage16;
}


///////////////////////////////////////////////////////////////////////
// 삐엠피 이미지 로뜨
//
BOOL ClImage::LoadBMP(LPSTR filename)
{
	FILE *fp;
	long m_numColors;
	long fileLength;
	int  sourceadd,targetadd;
	int  dummywidth;
	BOOL UPDOWN=FALSE;

	char ff[50];

	LPBITMAPINFO       m_pBmInfo;
	LPBITMAPINFOHEADER m_pBmInfoHeader;
	RGBQUAD           *m_pRGBTable;
	BYTE              *m_pDibBits;

	BITMAPFILEHEADER bmFileHeader;
	fp = fopen(filename,"rb");
    fread((void *)&bmFileHeader, sizeof(BITMAPFILEHEADER),1,fp);
	if (bmFileHeader.bfType != 0x4d42)
	{
		fclose(fp);
		return FALSE;
	}
	else
	{
		fileLength = filelength(fileno(fp));
        if (!fileLength) 
		{
			fclose(fp);
			MessageBox(NULL, "FileLength 0", MB_OK, NULL);
			return FALSE;
		}

		DWORD dibSize    = fileLength - sizeof(bmFileHeader);

		BYTE *pDib = new BYTE[dibSize];

		DWORD l = fread((void *)pDib, dibSize,1,fp);
        if (!l)
        {
			fclose(fp);
			delete pDib;
			wsprintf(ff,"BMP file Read Error : %s",filename);
            MessageBox(NULL, ff, MB_OK, NULL);
			return FALSE;
        }
        fclose(fp);
		m_pBmInfo = (LPBITMAPINFO) pDib;
		m_pBmInfoHeader = (LPBITMAPINFOHEADER) pDib;
		m_pRGBTable = (RGBQUAD *)(pDib + sizeof(BITMAPINFOHEADER));

		// Get color bit		
		if((m_pBmInfoHeader->biClrUsed == 0) && (m_pBmInfoHeader->biBitCount < 9))
			 m_numColors = (1 << m_pBmInfoHeader->biBitCount);
		else m_numColors = (int) m_pBmInfoHeader->biClrUsed;
		if(m_pBmInfoHeader->biHeight <0) 
		{
			m_pBmInfoHeader->biHeight = -m_pBmInfoHeader->biHeight;
			UPDOWN = TRUE;
		}
		// Get Image Size;
		if (m_pBmInfoHeader->biSizeImage == 0)
		{
	        m_pBmInfoHeader->biSizeImage = m_pBmInfoHeader->biWidth * m_pBmInfoHeader->biHeight;
			dummywidth = 0;
		} else 
		{
			dummywidth = (m_pBmInfoHeader->biSizeImage / m_pBmInfoHeader->biHeight) - m_pBmInfoHeader->biWidth;
		}
		if(m_pBmInfoHeader->biClrUsed == 0)
			m_pBmInfoHeader->biClrUsed = m_numColors;
		DWORD clrTableSize = m_numColors * sizeof(RGBQUAD);
		if(m_numColors > 256) clrTableSize = 0;
		m_pDibBits = pDib + m_pBmInfoHeader->biSize + clrTableSize;
		if(image != NULL) delete image;
		image = new RGBstruct[m_pBmInfoHeader->biWidth * m_pBmInfoHeader->biHeight];
		width = m_pBmInfoHeader->biWidth;				// 가상페이지의 가로 크기
		height = m_pBmInfoHeader->biHeight;				// 가상페이지의 세로 크기 
		sourceadd = 0;
		targetadd = 0;
		if(m_numColors>0)
		{
			for(int y = 0; y < m_pBmInfoHeader->biHeight; y++)
			{
				for(int x = 0; x < m_pBmInfoHeader->biWidth; x++)
				{
					image[targetadd].R = m_pRGBTable[m_pDibBits[sourceadd]].rgbRed;
					image[targetadd].B = m_pRGBTable[m_pDibBits[sourceadd]].rgbBlue;
					image[targetadd].G = m_pRGBTable[m_pDibBits[sourceadd]].rgbGreen;
					sourceadd++;
					targetadd++;
				}
				sourceadd+= dummywidth;
			}
		}
		else
		{
			for(int y = 0; y < m_pBmInfoHeader->biHeight; y++)
			{
				for(int x = 0; x < m_pBmInfoHeader->biWidth; x++)
				{
					image[targetadd].B = m_pDibBits[sourceadd];
					image[targetadd].G = m_pDibBits[sourceadd+1];
					image[targetadd].R = m_pDibBits[sourceadd+2];// b g r
					sourceadd+=3;
					targetadd++;
				}
				sourceadd+=dummywidth * 3;
			}
		
		}
	delete pDib;
	}
	if(!UPDOWN)
	{
		BYTE exR,exG,exB;
		int addr1,addr2;
		addr1=0;
		addr2=width * (height-1);
		for(int ayy=0;ayy<(height >> 1);ayy++)
		{
			for(int axx=0;axx<width;axx++)
			{
				exR = image[addr1+axx].R;
				exG = image[addr1+axx].G;
				exB = image[addr1+axx].B;
				image[addr1+axx].R = image[addr2+axx].R;
				image[addr1+axx].G = image[addr2+axx].G;
				image[addr1+axx].B = image[addr2+axx].B;
				image[addr2+axx].R = exR;
				image[addr2+axx].G = exG;
				image[addr2+axx].B = exB;
			}
			addr1+=width;
			addr2-=width;
		}
	}
	return TRUE;
}