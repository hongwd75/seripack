#include"stdafx.h"
#include"background.h"
#include<stdio.h>
#include<io.h>


extern POINT GetRegion(int x,int y,int width,int height);

void ErrorLog(char *filename,char *msg)
{
	FILE *fp;

	fp =fopen(filename,"at");
	fprintf(fp,msg);
	fprintf(fp,"\n");
	fclose(fp);
}

LRESULT CALLBACK DIB_WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);


int BG::m_MakeScrolBar = 0;

BG::BG(int Posx,int Posy,int ScSizeX,int ScSizeY,BOOL HaveScroll,HWND hWndParent,HINSTANCE hInst)
{

	// 차일드 윈도우 구성

	m_Enlarge		= 1;

	m_backimg		= NULL;
	m_virImg		= NULL;

	clicked			= FALSE;
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
						//WS_CHILD | WS_BORDER | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_THICKFRAME,
						WS_CHILD | WS_BORDER | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
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
	for(int i=0;i<ScSizeX*ScSizeY;i++)	{
		m_backimg[i].R = 0;
		m_backimg[i].G = 0;
		m_backimg[i].B = 100;
	}
	if(m_HaveScrol) MoveWindow(Dibhwnd,Posx,Posy,ScSizeX+18,ScSizeY+18,FALSE);
	else MoveWindow(Dibhwnd,Posx,Posy,ScSizeX+1,ScSizeY+1,FALSE);
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
	StretchDIBits(hdc,0,0,m_backround->bmiHeader.biWidth*m_Enlarge ,-(m_backround->bmiHeader.biHeight*m_Enlarge),//
		0,0,m_backround->bmiHeader.biWidth,-(m_backround->bmiHeader.biHeight), m_backimg, m_backround,DIB_RGB_COLORS,SRCCOPY);
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

void BG::CaptureImage2BG(int pointx,int pointy,int width1,int height1,BG *target)
{
	int loop1,loop2,saddr,taddr;
	RGBstruct *m_test = m_virImg;

	if(pointy<0) pointy=0;
	if(pointx<0) pointx=0;

	saddr = width * pointy + pointx;

	if(target->Is_Haveimage) 
	{
		target->Is_Haveimage = FALSE;
		delete (target->m_virImg);
	}

	taddr = 0;	
	target->m_virImg = new RGBstruct[width1 * height1];

	for(loop1=0;loop1<height1;loop1++)
	{
		for(loop2=0;loop2<width1;loop2++)
		{
			if(pointx + loop2 < width && pointy + loop1 < height)
			{
				target->m_virImg[taddr].R = m_test[saddr + loop2].R;
				target->m_virImg[taddr].G = m_test[saddr + loop2].G;
				target->m_virImg[taddr].B = m_test[saddr + loop2].B;
			} else {
				target->m_virImg[taddr].R = 0;
				target->m_virImg[taddr].G = 0;
				target->m_virImg[taddr].B = 0;
			}
			taddr++;
		}
		saddr +=width;
	}

	target->FillBackBuffer(0,0,100);
	target->Is_Haveimage = TRUE;
	target->width = width1;
	target->height = height1;
	target->m_ScreenNPos.x=0;
	target->m_ScreenNPos.y=0;
	target->WindowLen();
}


/*
 ====================================================================

  BMP    READER

 ====================================================================
*/
BOOL BG::ReadBMPfile(char *filename)
{

	FILE *fp;
	long m_numColors;
	long fileLength;
	int  sourceadd,targetadd;
	int  dummywidth;
	int  dWidth,dHeight;
	BOOL UPDOWN=FALSE;
	RGBstruct	*m_virImg2;

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
		m_pDibBits = pDib + m_pBmInfoHeader->biSize + clrTableSize;

		if(Is_Haveimage) 
		{
			Is_Haveimage = FALSE;
			delete m_virImg;
			delete MASK16;
		}
		if(m_pBmInfoHeader->biWidth % 16)
			 width = ((m_pBmInfoHeader->biWidth / 16) + 1) * 16;
		else width = m_pBmInfoHeader->biWidth;				// 가상페이지의 가로 크기

		if(m_pBmInfoHeader->biHeight % 16)
		 	 height = ((m_pBmInfoHeader->biHeight / 16) + 1) * 16;
		else height = m_pBmInfoHeader->biHeight;				// 가상페이지의 세로 크기 

		dWidth = m_pBmInfoHeader->biWidth;
		dHeight = m_pBmInfoHeader->biHeight;
		m_virImg = new RGBstruct[width * height];
		m_virImg2 = new RGBstruct[dHeight * dWidth];
		

        //  투명컬러로 채운다
		for(int i = 0; i < width * height; i++)
		{
			m_virImg[i].R = 255;
			m_virImg[i].G = 0;
			m_virImg[i].B = 255;
		}

		
		sourceadd = 0;
		targetadd = 0;
		if(m_numColors>0)
		{
			for(int y = 0; y < dHeight; y++)
			{
				for(int x = 0; x < dWidth; x++)
				{
					m_virImg2[targetadd].R = m_pRGBTable[m_pDibBits[sourceadd]].rgbRed;
					m_virImg2[targetadd].B = m_pRGBTable[m_pDibBits[sourceadd]].rgbBlue;
					m_virImg2[targetadd].G = m_pRGBTable[m_pDibBits[sourceadd]].rgbGreen;
					sourceadd++;
					targetadd++;
				}
				sourceadd+= dummywidth;
				//targetadd+= addsumx;
			}
		}
		else
		{
			for(int y = 0; y < dHeight; y++)
			{
				for(int x = 0; x < dWidth; x++)
				{
					m_virImg2[targetadd].B = m_pDibBits[sourceadd];
					m_virImg2[targetadd].G = m_pDibBits[sourceadd+1];
					m_virImg2[targetadd].R = m_pDibBits[sourceadd+2];// b g r
					sourceadd+=3;
					targetadd++;
				}
				sourceadd+=dummywidth * 3;
			}

		//	for(i = 0; i < m_pBmInfoHeader->biSizeImage; i++)
		//	{
		//		m_virImg2[i].B = m_pRGBTable[m_pDibBits[i*3]].rgbBlue;
		//		m_virImg2[i].G = m_pRGBTable[m_pDibBits[i*3+1]].rgbGreen;
		//		m_virImg2[i].R = m_pRGBTable[m_pDibBits[i*3+2]].rgbRed;
		//	}
		}
		delete pDib;
	}

	if(!UPDOWN)
	{
		BYTE exR,exG,exB;
		int addr1,addr2;
		addr1=0;
		addr2=dWidth * (dHeight-1);
		for(int ayy=0;ayy < (dHeight >> 1);ayy++)
		{
			for(int axx=0;axx < dWidth;axx++)
			{
				exR = m_virImg2[addr1+axx].R;
				exG = m_virImg2[addr1+axx].G;
				exB = m_virImg2[addr1+axx].B;

				m_virImg2[addr1+axx].R = m_virImg2[addr2+axx].R;
				m_virImg2[addr1+axx].G = m_virImg2[addr2+axx].G;
				m_virImg2[addr1+axx].B = m_virImg2[addr2+axx].B;

				m_virImg2[addr2+axx].R = exR;
				m_virImg2[addr2+axx].G = exG;
				m_virImg2[addr2+axx].B = exB;

			}
			addr1+=dWidth;
			addr2-=dWidth;

		}
	}

	int ss1=0,ss2=0;
	for(int a1= 0 ; a1 < dHeight; a1++)
	{
		for(int a2 = 0; a2 < dWidth; a2++)
		{
			m_virImg[ss1+a2].R = m_virImg2[ss2+a2].R;
			m_virImg[ss1+a2].G = m_virImg2[ss2+a2].G;
			m_virImg[ss1+a2].B = m_virImg2[ss2+a2].B;
		}
		ss1+=width;
		ss2+=dWidth;
	}

	delete m_virImg2;

	Is_Haveimage = TRUE;
	m_ScreenNPos.x = 0;
	m_ScreenNPos.y = 0;
	WindowLen();

	maskwidth = width >> 4;
	maskheight = height >> 4;
	MASK16 = new BYTE[maskwidth * maskheight];
	memset(MASK16,0,maskwidth * maskheight);
	FillBackBuffer(0,0,100);
	//--------------------------
	m_PutImage2BackBuffer();
	//--------------------------
	return TRUE;
}



/*
 ====================================================================

  TIF    READER

 ====================================================================
*/
BOOL BG::ReadTIFfile(char *filename)
{
	TIFF *tiffile;
	tiffile = new TIFF(hwndParent);

	if(tiffile->LoadTIFF(filename))
	{
		if(Is_Haveimage) 
		{
			Is_Haveimage = FALSE;
			delete m_virImg;
		}
		width = tiffile->Width;
		height = tiffile->Height;
		m_virImg = new RGBstruct[width * height];
		for(int loop=0;loop < width * height;loop++)
		{
			m_virImg[loop].R = tiffile->Image[loop].R;
			m_virImg[loop].G = tiffile->Image[loop].G;
			m_virImg[loop].B = tiffile->Image[loop].B;
		}
		Is_Haveimage = TRUE;
		m_ScreenNPos.x = 0;
		m_ScreenNPos.y = 0;
		WindowLen();
	} else FALSE;
	delete tiffile;
	return TRUE;
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


/*
=================================================================
     For Image Processing Program
=================================================================
*/

void BG::DrawBox(int sx,int sy,int fx,int fy,int R,int G,int B)
{
	int loop;
	int addr1,addr2;
	int rsx,rsy,rfx,rfy;

	rsx = sx - m_ScreenNPos.x;
	rsy = sy - m_ScreenNPos.y;
	rfx = fx - m_ScreenNPos.x;
	rfy = fy - m_ScreenNPos.y;

//	if(rfx <= m_ScreenSize.x) rfx = m_ScreenSize.x-1;
//	if(rfy <= m_ScreenSize.y) rfy = m_ScreenSize.y-1;

	addr1=m_ScreenSize.x * rsy;
	addr2=m_ScreenSize.x * rfy;
	for(loop=rsx;loop<=rfx;loop++)
	{
		if(loop>=0 && loop < m_ScreenSize.x)
		{
			if(rsy>=0 && rsy < m_ScreenSize.y)
			{
				m_backimg[addr1+loop].R = R;
				m_backimg[addr1+loop].G = G;
				m_backimg[addr1+loop].B = B;
			}
			if(rfy>=0 && rfy < m_ScreenSize.y)
			{
				m_backimg[addr2+loop].R = R;
				m_backimg[addr2+loop].G = G;
				m_backimg[addr2+loop].B = B;
			}
		}
	}				/* x line */

	addr1=m_ScreenSize.x * rsy + rsx;
	addr2=m_ScreenSize.x * rsy + rfx;
	for(loop=rsy;loop<=rfy;loop++)
	{
		if(loop>=0 && loop < m_ScreenSize.y)
		{
			if(rsx>=0 && rsx < m_ScreenSize.x)
			{
				m_backimg[addr1].R = R;
				m_backimg[addr1].G = G;
				m_backimg[addr1].B = B;
			}
			if(rfx>=0 && rfx < m_ScreenSize.x)
			{
				m_backimg[addr2].R = R;
				m_backimg[addr2].G = G;
				m_backimg[addr2].B = B;
			}
		}
		addr1+=m_ScreenSize.x;
		addr2+=m_ScreenSize.x;
	}              /* y line */
}


int BG::GetMasked(int x,int y)
{
    int addr=0;
	int sum = 0;
    addr = y * width + x;

	for(int i = 0; i < 16; i++)
	{
	  for(int j = 0; j < 16; j++)
	  {
		 if(!(m_virImg[addr+j].R == 255 && m_virImg[addr+j].B == 255 && m_virImg[addr+j].G == 0)) sum++;
	  }
	  addr+=width;
	}

    if(sum > 128) return 1;
		   else return 0;
}


void BG::AutoMask(void)
{
   int	loop1=0;
   for(int sy = 0; sy < height; sy+=16)
   {
     for(int sx = 0; sx < width; sx+=16)
	 {
	  MASK16[loop1] = GetMasked(sx,sy);
	  loop1++;
	 }
   } 
}


void BG::PutMask(int x,int y)
{
	if( this->maskwidth < x ) return;
	if( this->maskheight < y ) return;

	MASK16[y * maskwidth + x] = 1 - MASK16[y * maskwidth + x];
}

void BG::ShowMask(void)
{
//m_backimg
	int add=0;
	for(int yy = 0; yy < maskheight; yy++)
	{
		for(int xx = 0; xx < maskwidth; xx++)
		{
			if(MASK16[add])
			{
				int iadd = (yy * 16) * m_ScreenSize.x + (xx*16);
				for(int i = 0; i < 16; i++)
				{
					for(int j = 0; j < 16; j++)
					{
						int k = (m_backimg[iadd+j].R + m_backimg[iadd+j].G + m_backimg[iadd+j].B) / 3;
						m_backimg[iadd+j].R = 100;
						m_backimg[iadd+j].G = k;
						m_backimg[iadd+j].B = 0;
					}
					iadd+=m_ScreenSize.x;
				}
			}
			add++;
		}
	}
}
/*
=================================================================
     
=================================================================
*/


LRESULT CALLBACK DIB_WndProc(HWND hwnD, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	BG *Win = (BG *)GetWindowLong(hwnD, GWL_USERDATA);
	static BOOL dokidoki=FALSE;
	static HPEN hPen;
	static HPEN hPen2;

	switch(iMsg)
	{

		case WM_CREATE:
			hPen = CreatePen(PS_SOLID,1,RGB(0,100,0));
			break;


		case WM_COMMAND :
			if(Win == NULL) return 0;
			//ShowScrollBar(hwnd,SB_VERT,TRUE);
			//ShowScrollBar(hwnd,SB_HORZ,TRUE);
			return 0;

		case WM_SIZE :
			{
				if(Win == NULL) return 0;
				//Win->WindowReSize();
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
			
		case WM_LBUTTONDOWN:
			{
				POINT a;
				a.x = LOWORD(lParam)+Win->m_ScreenNPos.x;
				a.y = HIWORD(lParam)+Win->m_ScreenNPos.y;

				a.x = a.x >> 4;
				a.y = a.y >> 4;
				Win->PutMask(a.x,a.y);	
				InvalidateRect(hwnD,NULL,FALSE);	
			}
			break;


		case WM_PAINT :
			{
				RECT rect;
				PAINTSTRUCT ps;
				Win->m_PutImage2BackBuffer();
				BeginPaint(Win->Dibhwnd,&ps);
				GetClientRect(hwnD,&rect);
				if(Win->Is_Haveimage)
				{
					Win->ShowMask();
				}
				Win->PutBackGround(ps.hdc);
				SelectObject(ps.hdc,hPen);
				for(int j = 15; j <480; j+=16)
				{
					MoveToEx(ps.hdc,0,j,NULL);
					LineTo(ps.hdc,640,j);
				}
				for(int j2 = 15; j2 <640; j2+=16)
				{
					MoveToEx(ps.hdc,j2,0,NULL);
					LineTo(ps.hdc,j2,480);
				}

				EndPaint(Win->Dibhwnd,&ps);
			}
			return 0;

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



/*===================================================================
     TIF functions
=====================================================================*/
	TIFF::TIFF(HWND ff)
	{
		hwND = ff;
		Image = NULL;
		m_IFDindex = NULL;
	}

	TIFF::~TIFF()
	{
		if(Image != NULL) delete Image;
		if(m_IFDindex != NULL) delete m_IFDindex;
	}

	BOOL TIFF::LoadTIFF(char *filename)
	{
		FILE *fp;
		int i,j;
		int BitsPerSample;

		WORD RedPal[256];
		WORD GreenPal[256];
		WORD BluePal[256];

		DWORD MemSize;

		int NoOfStripByteCounts = 0;
		int NoOfStripOffsets    = 0;
        int FCountStripInfo     = 0;

		BYTE *ImageData;	            /* For Image Data */
		TStripInfo *TListStripInfo = NULL;		

		fp = fopen(filename,"rb");

		fread(&m_header,sizeof(TIFHEADER),1,fp);
	

		if(m_header.Endian != 0x4949 && m_header.Endian != 0x4D4D)
		{
			fclose(fp);
			return FALSE;   // Error Check
		}

		fseek(fp,m_header.FirstIFD,SEEK_SET);
		fread(&IFDcount,2,1,fp);
		m_IFDindex = new IFDHEADER[IFDcount]; 

		for(i=0;i<IFDcount;i++)
		{
			fread(&m_IFDindex[i],sizeof(IFDHEADER),1,fp);
			if(m_IFDindex[i].Tag == tiffTag_StripByteCounts)
			{
				NoOfStripByteCounts = m_IFDindex[i].Count;
			}
			if(m_IFDindex[i].Tag == tiffTag_StripOffsets)
			{
				NoOfStripOffsets = m_IFDindex[i].Count;
			}
		}

/*
=====================================================================
	Offset Set
=====================================================================
*/
		if(NoOfStripByteCounts == NoOfStripOffsets)
		{
			FCountStripInfo = NoOfStripByteCounts;
			TListStripInfo = new TStripInfo[FCountStripInfo];
			for(i=0;i<IFDcount;i++)
			{
				// Strip Byte Counts
				if(m_IFDindex[i].Tag == tiffTag_StripByteCounts)
				{
					if(m_IFDindex[i].Count == 1)
					{
						TListStripInfo[0].StripByteCounts = m_IFDindex[i].Value;
					} else 
		       		// Scan the file for the list of Strip Bytes Counts
					{
						fseek(fp,m_IFDindex[i].Value,SEEK_SET);
						for(j=0;j<FCountStripInfo;j++)
						{
							fread(&TListStripInfo[j].StripByteCounts,4,1,fp);
						}
					}
				}
				// StripOffsets
				if (m_IFDindex[i].Tag == tiffTag_StripOffsets)
				{
					if(m_IFDindex[i].Count == 1)
					{
						TListStripInfo[0].StripOffsets = m_IFDindex[i].Value;
					} else 
					{
						fseek(fp,m_IFDindex[i].Value,SEEK_SET);
						for(j=0;j<FCountStripInfo;j++)
						{
							fread(&TListStripInfo[j].StripOffsets,4,1,fp);
						}
					}
				}
			}
		}
		MemSize = 0;
		for(i=0;i<FCountStripInfo;i++)
		{
			MemSize+=MemSize+TListStripInfo[i].StripByteCounts;
		}
		ImageData = new BYTE[MemSize];
		BYTE *DumpPoint = ImageData;			// Same Pointer
		for(i=0;i<FCountStripInfo;i++)
		{
		  fseek(fp,TListStripInfo[i].StripOffsets,SEEK_SET);
		  fread((BYTE *)DumpPoint,TListStripInfo[i].StripByteCounts,1,fp);
		  DumpPoint+=TListStripInfo[i].StripByteCounts;	// Point Add
		}


/*
=====================================================================
	Image Size , Bit Set
=====================================================================
*/
		if(GetIFD_Index(tiffTag_ImageWidth)) Width = IFDAddr.Value;
		else 
		{
			MessageBox(hwND,"Error reading tag Image Width","TIF Error",MB_OK | MB_ICONSTOP);
			return FALSE;
		}
		if(GetIFD_Index(tiffTag_ImageLength)) Height = IFDAddr.Value;
		else 
		{
			MessageBox(hwND,"Error reading tag Image Height","TIF Error",MB_OK | MB_ICONSTOP);
			return FALSE;
		}

		if(GetIFD_Index(tiffTag_BitsPerSample)) BitsPerSample = IFDAddr.Value;
		else 
		{
			MessageBox(hwND,"Error reading tag BitsPerSample","TIF Error",MB_OK | MB_ICONSTOP);
			return FALSE;
		}
		if(GetIFD_Index(tiffTag_PhotometricInterpretation))
		{
			if(Image != NULL) delete Image;
			Image = new RGB8[Width * Height];
			switch(IFDAddr.Value)
			{
				case 0:		/* White is Zero */
					for(i=0;i<256;i++)
					{
						PAL[i].R = 255 - i;
						PAL[i].G = 255 - i;
						PAL[i].B = 255 - i;
					}
					if(BitsPerSample == 4)
					{
						for(i=0;i<256;i++)
						{
							PAL[i].R = (255 - i) * 255 / 15;
							PAL[i].G = (255 - i) * 255 / 15;
							PAL[i].B = (255 - i) * 255 / 15;
						}
					}
					IndexImageTo24BitImage(ImageData);
				break;
				case 1:		/* Black is Zero */
					for(i=0;i<256;i++)
					{
						PAL[i].R = i;
						PAL[i].G = i;
						PAL[i].B = i;
					}
					if(BitsPerSample == 4)
					{
						for(i=0;i<256;i++)
						{
							PAL[i].R = (i) * 255 / 15;
							PAL[i].G = (i) * 255 / 15;
							PAL[i].B = (i) * 255 / 15;
						}
					}
					IndexImageTo24BitImage(ImageData);
				break;
				case 2:		/* RGB color     */
					short  BitsPerSampleRed;
					short  BitsPerSampleGreen;
					short  BitsPerSampleBlue;
					if(GetIFD_Index(tiffTag_BitsPerSample))
					{
						if(IFDAddr.Count == 1)
						{
						    BitsPerSampleRed   = IFDAddr.Value;
							BitsPerSampleGreen = IFDAddr.Value;
							BitsPerSampleBlue  = IFDAddr.Value;
						} else 
						{
							fseek(fp,IFDAddr.Value,SEEK_SET);
							fread(&BitsPerSampleRed,2,1,fp);
							fread(&BitsPerSampleGreen,2,1,fp);
							fread(&BitsPerSampleBlue,2,1,fp);
						}
					    if (BitsPerSampleRed != 8 && BitsPerSampleGreen != 8 && BitsPerSampleBlue != 8)
						{
							MessageBox(hwND,"Error: Bits Per Sample (RGB) not 8","TIF Error",MB_OK | MB_ICONSTOP);
							return FALSE;
						}

						for(i=0;i<Width * Height;i++)
						{
							Image[i].R = ImageData[3*i];
							Image[i].G = ImageData[3*i+1];
							Image[i].B = ImageData[3*i+2];
						}
					} else
					{
							MessageBox(hwND,"Error reading tag Bits Per Sample","TIF Error",MB_OK | MB_ICONSTOP);
							return FALSE;
					}

				break;
				case 3:		/* Index color   */
					if(GetIFD_Index(tiffTag_ColorMapOffset))
					{
						fseek(fp,IFDAddr.Value,SEEK_SET);
						fread(&RedPal,IFDAddr.Count*2,1,fp);
						fread(&GreenPal,IFDAddr.Count*2,1,fp);
						fread(&BluePal,IFDAddr.Count*2,1,fp);
						for(i=0;i<IFDAddr.Count;i++)
						{
							PAL[i].R = RedPal[i] >> 8;
							PAL[i].G = GreenPal[i] >> 8;
							PAL[i].B = BluePal[i] >> 8;
						}
					}
					IndexImageTo24BitImage(ImageData);
				break;
			}
		}
		else 
		{
			MessageBox(hwND,"Error reading tag PhotometricInterpretation","TIF Error",MB_OK | MB_ICONSTOP);
			return FALSE;
		}

		// Image Data Load (first image only!)
		fclose(fp);

		delete ImageData;
		delete TListStripInfo;
		return TRUE;
	}

	BOOL TIFF::GetIFD_Index(int Index)
	{
		for(int i = 0; i < IFDcount; i++)
		{
			if(m_IFDindex[i].Tag == Index)
			{
				IFDAddr = m_IFDindex[i];
				return TRUE;
			}
		}
		return FALSE;
	}



	void TIFF::IndexImageTo24BitImage(BYTE *src)
	{
		BYTE *temp = src;
		int loop;

		for(loop=0;loop<Width * Height;loop++)
		{
			Image[loop].R = PAL[temp[loop]].R;
			Image[loop].G = PAL[temp[loop]].G;
			Image[loop].B = PAL[temp[loop]].B;
		}
         
	}

