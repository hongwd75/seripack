/*
   윈도우용 그래픽 클래스
   16Bit Color 모드용으로 되어있다.
*/

#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__
#include"stdafx.h"



#define		USER_GETPOS				0x5FF0

typedef struct {
	unsigned char B;
	unsigned char G;
	unsigned char R;
} RGBstruct;



class ClImage {
	public:
		RGBstruct	*image;
		int			width;
		int			height;

		ClImage();
		ClImage(LPSTR filename);
		~ClImage();

		WORD *Get16BitImage();
		BOOL  LoadBMP(LPSTR filename);
};

class BG {

	private:
		static int m_MakeScrolBar;

	public:
		HWND				Dibhwnd;
		HWND				hwndParent;

	public:
		int					width;				// 가상페이지의 가로 크기
		int					height;				// 가상페이지의 세로 크기 
		RGBstruct			*m_virImg;			// 가상페이지의 이미지 데이타

		RGBstruct			*m_backimg;
		
		void m_PutImage2BackBuffer(void);							// 이미지를 백버퍼에 넣는다.
		

	public:
		HANDLE				m_back_handle;
		LPBITMAPINFO		m_backround;


		POINT				m_ScreenPos;		// 스크린 위치
		POINT				m_ScreenSize;		// 스크린 사이즈
		POINT				m_ScreenNPos;		// 스크린 현 위치 


		POINT				m_SP;				// SP 좌표
		BOOL				m_HaveScrol;		// 스크롤바를 가지고 있다.
		BOOL				Is_Haveimage;		// 이미지가 로드 되면 true값을 가진다.
		BOOL                CLOSE;				//
		int                 m_ScMaxH;			// 세로 스크롤바 최대 크기 
		int					m_ScMaxW;			// 가로 스크롤바 최대 크기
		int                 m_ScPosH;			// 세로 스크롤바 현 위치
		int					m_ScPosW;			// 가로 스크롤바 현 위치

		int					m_Enlarge;			// 확대 비율(최초 x1)
		char				szName[30];



		BOOL				m_ViewCross;		// 십자가를 화면에 넣을것인지 결정(월드 좌표로 쓰인다.)
		BOOL				m_ImageCurs;		// 모션별 자신의 좌표.
		BOOL				m_resizewindow;		// preview용
		POINT				Worldpoint;			// 월드 포인트
		POINT				Addpoint;			// 이미지별 포인트(월드 포인트에 더해진다)
		POINT				m_ImageSize;		// 이미지 사이즈
		RGBstruct			*m_realvir;
		POINT				realvirsize;

	public:
		~BG();
		BG(int Posx,int Posy,int ScSizeX,int ScSizeY,BOOL HaveScroll,HWND hWndParent,HINSTANCE hInst);
		void ResetViewSize(int Posx,int Posy,int ScSizeX,int ScSizeY);

	public:
		void PutDataVirMemory(BYTE rgb,int addr);
		void PutImage(int x,int y, int width1,int height1, RGBstruct *src);
		void PutImageInWorld(int x,int y, int width1,int height1, RGBstruct *src);
		void MakeVirMemory(int x,int y);
		void WindowReSize();
		void PutBackGround(HDC hdc);
		void FillBackBuffer(int r,int g,int b);
		void ResetScroll();
		void WindowLen(void);

		void WindowEnlarge(void);
		void WindowReduce(void);
};
#endif