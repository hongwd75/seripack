/*
   ������� �׷��� Ŭ����
   16Bit Color �������� �Ǿ��ִ�.
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
		int					width;				// ������������ ���� ũ��
		int					height;				// ������������ ���� ũ�� 
		RGBstruct			*m_virImg;			// ������������ �̹��� ����Ÿ

		RGBstruct			*m_backimg;
		
		void m_PutImage2BackBuffer(void);							// �̹����� ����ۿ� �ִ´�.
		

	public:
		HANDLE				m_back_handle;
		LPBITMAPINFO		m_backround;


		POINT				m_ScreenPos;		// ��ũ�� ��ġ
		POINT				m_ScreenSize;		// ��ũ�� ������
		POINT				m_ScreenNPos;		// ��ũ�� �� ��ġ 


		POINT				m_SP;				// SP ��ǥ
		BOOL				m_HaveScrol;		// ��ũ�ѹٸ� ������ �ִ�.
		BOOL				Is_Haveimage;		// �̹����� �ε� �Ǹ� true���� ������.
		BOOL                CLOSE;				//
		int                 m_ScMaxH;			// ���� ��ũ�ѹ� �ִ� ũ�� 
		int					m_ScMaxW;			// ���� ��ũ�ѹ� �ִ� ũ��
		int                 m_ScPosH;			// ���� ��ũ�ѹ� �� ��ġ
		int					m_ScPosW;			// ���� ��ũ�ѹ� �� ��ġ

		int					m_Enlarge;			// Ȯ�� ����(���� x1)
		char				szName[30];



		BOOL				m_ViewCross;		// ���ڰ��� ȭ�鿡 ���������� ����(���� ��ǥ�� ���δ�.)
		BOOL				m_ImageCurs;		// ��Ǻ� �ڽ��� ��ǥ.
		BOOL				m_resizewindow;		// preview��
		POINT				Worldpoint;			// ���� ����Ʈ
		POINT				Addpoint;			// �̹����� ����Ʈ(���� ����Ʈ�� ��������)
		POINT				m_ImageSize;		// �̹��� ������
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